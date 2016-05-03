#include <exception>

#include "OcclusionLightLeaf.hpp"
#include "Scene.hpp"

RT::OcclusionLightLeaf::OcclusionLightLeaf(RT::Color const & color, double radius)
  : _color(color), _radius(radius)
{
  if (_radius < 0.f)
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
}

RT::OcclusionLightLeaf::~OcclusionLightLeaf()
{}

RT::Color RT::OcclusionLightLeaf::preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const & intersection, unsigned int, unsigned int) const
{
  return intersection.material.color * intersection.material.direct.ambient * _color;
}

RT::Color RT::OcclusionLightLeaf::render(Math::Matrix<4, 4> const &, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int) const
{
  // If no ambient light, stop
  if (intersection.material.direct.ambient == 0.f || intersection.material.transparency.intensity == 1.f || intersection.material.reflection.intensity == 1.f)
    return RT::Color(0.f);

  unsigned int	quality = intersection.material.direct.quality > recursivite ? intersection.material.direct.quality - recursivite : 0;

  // If quality to basic
  if (quality <= 1 || _radius == 0.f)
    return intersection.material.color * intersection.material.direct.ambient * (1.f - intersection.material.transparency.intensity) * (1.f - intersection.material.reflection.intensity);

  // Inverse normal if necessary
  Math::Vector<4>	n = intersection.normal.d();
  if (RT::Ray::cos(ray.d(), intersection.normal.d()) > 0)
    n *= -1;

  // Calculate rotation angles of normal
  double	ry = -std::asin(n.z());
  double	rz = 0;

  if (n.x() != 0 || n.y() != 0)
    rz = n.y() > 0 ?
    +std::acos(n.x() / std::sqrt(n.x() * n.x() + n.y() * n.y())) :
    -std::acos(n.x() / std::sqrt(n.x() * n.x() + n.y() * n.y()));
  
  // Rotation matrix to get ray to point of view
  Math::Matrix<4, 4>  matrix = Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz));

  // Point origin right above intersection
  Math::Vector<4>	p = intersection.normal.p() + n * Math::Shift;

  // Generate and render occlusion rays
  unsigned int	nb_ray = 0;
  RT::Color	ambient = 0.f;

  for (double a = Math::Random::rand(Math::Pi / (2.f * quality)); a < Math::Pi / 2.f; a += Math::Pi / (2.f * quality))
    for (double b = Math::Random::rand(2.f * Math::Pi / (std::cos(a) * quality * 2.f + 1.f)); b < 2.f * Math::Pi; b += (2.f * Math::Pi) / (std::cos(a) * quality * 2.f + 1.f))
    {
      // Calculate ray according to point on the hemisphere
      std::list<RT::Intersection> intersect = scene->csg()->render(RT::Ray(p, matrix * Math::Vector<4>(std::sin(a), std::cos(b) * std::cos(a), std::sin(b) * std::cos(a), 0.f)).normalize());
      
      // Occlusion above
      if (intersection.material.transparency.intensity != 1.f)
      {
	std::list<RT::Intersection>::const_iterator it = intersect.begin();
	RT::Color				    light = _color;

	while (it != intersect.end() && +it->distance < 0.f)
	  it++;
	while (it != intersect.end() && +it->distance < _radius && light != 0.f)
	{
	  // NOTE: occlusion ray is not reflected
	  ambient += RT::Color(+it->distance / _radius) * light * it->material.color * (1.f - it->material.transparency.intensity) * (1.f - intersection.material.transparency.intensity);
	  light *= it->material.color * it->material.transparency.intensity;
	  it++;
	}
	ambient += light * (1.f - intersection.material.transparency.intensity);
      }

      // Occlusion transparency
      if (intersection.material.transparency.intensity != 0.f)
      {
	std::list<RT::Intersection>::const_reverse_iterator it = intersect.rbegin();
	RT::Color					    light = _color;
	
	while (it != intersect.rend() && -it->distance < 0.f)
	  it++;
	while (it != intersect.rend() && -it->distance < _radius && light != 0.f)
	{
	  // NOTE: occlusion ray is not reflected
	  ambient += RT::Color(-it->distance / _radius) * light * it->material.color * (1.f - it->material.transparency.intensity) * (intersection.material.transparency.intensity);
	  light *= it->material.color * it->material.transparency.intensity;
	  it++;
	}
	ambient += light * (intersection.material.transparency.intensity);
      }

      nb_ray++;
    }

  return ambient / nb_ray * intersection.material.color * intersection.material.direct.ambient * (1.f - intersection.material.transparency.intensity) * (1.f - intersection.material.reflection.intensity);
}
