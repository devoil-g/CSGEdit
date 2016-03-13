#include <list>
#include <sstream>

#include "DirectionalLightLeaf.hpp"
#include "Exception.hpp"
#include "Math.hpp"
#include "Scene.hpp"

RT::DirectionalLightLeaf::DirectionalLightLeaf(RT::Color const & color, double angle)
  : _color(color), _angle(angle)
{
  // Check values
  if (_angle < 0.f || _angle >= 90.f)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}

RT::DirectionalLightLeaf::~DirectionalLightLeaf()
{}

RT::Color RT::DirectionalLightLeaf::preview(Math::Matrix<4, 4> const & transformation, RT::Scene const *, RT::Ray const &, RT::Intersection const & intersection, unsigned int) const
{
  // If no ambient light, stop
  if (intersection.material.color == 0.f || intersection.material.light.diffuse == 0.f)
    return RT::Color(0.f);

  return intersection.material.color * intersection.material.light.diffuse * _color * std::fmax(RT::Ray::cos(intersection.normal.d(), Math::Vector<4>(transformation * Math::Vector<4>(-1.f, 0.f, 0.f, 0.f))), 0.f);
}

RT::Color RT::DirectionalLightLeaf::render(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int) const
{
  if (intersection.material.light.diffuse == 0.f && intersection.material.light.specular == 0.f)
    return RT::Color(0.f);

  // Inverse normal if necessary
  bool			inside = false;
  Math::Vector<4>	n = intersection.normal.d();
  if (RT::Ray::cos(ray.d(), intersection.normal.d()) > 0)
  {
    n *= -1.f;
    inside = true;
  }
  
  std::list<RT::Ray>	rays;

  Math::Vector<4>	p = intersection.normal.p() + n * Math::Shift;
  if (intersection.material.light.quality <= 1 || _angle == 0)
    rays.push_back(RT::Ray(p, transformation * Math::Vector<4>(-1.f, 0.f, 0.f, 0.f)));
  else
    for (double a = Math::Random::rand(1.f / (intersection.material.light.quality + 1)); a < 1.f; a += 1.f / (intersection.material.light.quality + 1))
      for (double b = Math::Random::rand((2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f)); b < 2.f * Math::Pi; b += (2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f))
	rays.push_back(RT::Ray(p, transformation * Math::Vector<4>(-1.f / std::tan(Math::Utils::DegToRad(_angle)), a * std::cos(b), a * std::sin(b), 0.f)));

  // Calculate reflection ray
  Math::Vector<4>	r = intersection.normal.p() - ray.p() - n * 2.f * (n.x() * (intersection.normal.p().x() - ray.p().x()) + n.y() * (intersection.normal.p().y() - ray.p().y()) + n.z() * (intersection.normal.p().z() - ray.p().z())) / (n.x() * n.x() + n.y() * n.y() + n.z() * n.z());
  
  // Render generated rays
  RT::Color		diffuse, specular;
  for (RT::Ray const & it : rays)
  {
    std::list<RT::Intersection>	intersect = scene->csg()->render(it);
    RT::Color			light = _color;
    
    // Render light
    while (!intersect.empty() && intersect.front().distance < 0.f)
      intersect.pop_front();
    while (!intersect.empty() && light != 0.f)
    {
      light *= intersect.front().material.color * intersect.front().material.transparency.intensity;
      intersect.pop_front();
    }

    // Apply light to diffuse component
    diffuse += light * std::fabs(RT::Ray::cos(n, it.d()));

    // Apply light to specular component
    if (inside == false)
      specular += light * std::pow(std::fmax(RT::Ray::cos(r, it.d()), 0.f), intersection.material.light.shininess);
  }

  return diffuse / (double)rays.size() * intersection.material.color * intersection.material.light.diffuse * (1.f - intersection.material.transparency.intensity) * (1.f - intersection.material.reflection.intensity)
    + specular / (double)rays.size() * intersection.material.light.specular;
}
