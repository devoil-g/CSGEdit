#include <sstream>

#include "Math.hpp"
#include "OcclusionLight.hpp"
#include "Scene.hpp"

RT::OcclusionLight::OcclusionLight(RT::Color const & color, double radius, unsigned int quality)
  : _color(color), _radius(radius), _quality(quality)
{
  _radius = _radius > 0.f ? _radius : 0.f;
  _quality = _quality > 1 ? _quality : 1;
}

RT::OcclusionLight::~OcclusionLight()
{}

RT::Color RT::OcclusionLight::preview(RT::Scene const * scene, RT::Ray const & ray, RT::Ray const & normal, RT::Material const & material) const
{
  return material.color * material.ambient * _color * scene->config().lightAmbient;
}

RT::Color RT::OcclusionLight::render(RT::Scene const * scene, RT::Ray const & ray, RT::Ray const & normal, RT::Material const & material) const
{
  // If no ambient light, stop
  if (scene->config().lightAmbient == 0.f || material.ambient == 0.f || material.transparency == 1.f || material.reflection == 1.f)
    return RT::Color(0.f);

  // If quality to basic
  if (_quality <= 1 || _radius == 0.f)
    return material.color * scene->config().lightAmbient * material.ambient * (1.f - material.transparency) * (1.f - material.reflection);

  RT::Ray	occ;

  // Inverse normal if necessary
  RT::Ray	n = normal;
  if (RT::Ray::cos(ray, normal) > 0)
    n.d() *= -1;

  // Calculate rotation angles of normal
  double	ry = -std::asin(n.d().z()), rz;

  if (n.d().x() != 0 || n.d().y() != 0)
    rz = n.d().y() > 0 ?
    +std::acos(n.d().x() / std::sqrt(n.d().x() * n.d().x() + n.d().y() * n.d().y())) :
    -std::acos(n.d().x() / std::sqrt(n.d().x() * n.d().x() + n.d().y() * n.d().y()));
  else
    rz = 0;

  // Rotation matrix to get ray to point of view
  Math::Matrix<4, 4>  matrix = Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz));

  // Point origin right above intersection
  occ.p() = n.p() + n.d() * Math::Shift;
  
  // Generate and render occlusion rays
  unsigned int	nb_ray = 0;
  RT::Color	ambient = 0.f;

  for (double a = Math::Random::rand(Math::Pi / (2.f * _quality)); a < Math::Pi / 2.f; a += Math::Pi / (2.f * _quality))
    for (double b = Math::Random::rand(2.f * Math::Pi / (std::cos(a) * _quality * 2.f + 1.f)); b < 2.f * Math::Pi; b += (2.f * Math::Pi) / (std::cos(a) * _quality * 2.f + 1.f))
    {
      // Calculate ray according to point on the hemisphere
      occ.d().x() = std::sin(a);
      occ.d().y() = std::cos(b) * std::cos(a);
      occ.d().z() = std::sin(b) * std::cos(a);
      occ.d() = matrix * occ.d();
      
      std::list<RT::Intersection> intersect = scene->tree()->render(occ.normalize());
      
      // Occlusion above
      if (material.transparency != 1.f)
      {
	std::list<RT::Intersection>::const_iterator it = intersect.begin();
	RT::Color				    light = RT::Color(_color);

	while (it != intersect.end() && +it->distance < 0.f)
	  it++;
	while (it != intersect.end() && +it->distance < _radius && light != RT::Color(0.f))
	{
	  // NOTE: occlusion ray is not reflected
	  ambient += RT::Color(+it->distance / _radius) * light * it->material.color * (1.f - it->material.transparency) * (1.f - material.transparency);
	  light *= it->material.color * it->material.transparency;
	  it++;
	}
	ambient += light * (1.f - material.transparency);
      }

      // Occlusion transparency
      if (material.transparency != 0.f)
      {
	std::list<RT::Intersection>::const_reverse_iterator it = intersect.rbegin();
	RT::Color					    light = RT::Color(_color);
	
	while (it != intersect.rend() && -it->distance < 0.f)
	  it++;
	while (it != intersect.rend() && -it->distance < _radius && light != RT::Color(0.f))
	{
	  // NOTE: occlusion ray is not reflected
	  ambient += RT::Color(-it->distance / _radius) * light * it->material.color * (1.f - it->material.transparency) * (material.transparency);
	  light *= it->material.color * it->material.transparency;
	  it++;
	}
	ambient += light * (material.transparency);
      }

      nb_ray++;
    }

  return ambient / nb_ray * scene->config().lightAmbient * material.color * material.ambient * (1.f - material.transparency) * (1.f - material.reflection);
}

std::string	RT::OcclusionLight::dump() const
{
  std::stringstream	stream;

  stream << "occlusion_light(" << _color.dump() << ", " << _radius << ", " << _quality << ");";

  return stream.str();
}