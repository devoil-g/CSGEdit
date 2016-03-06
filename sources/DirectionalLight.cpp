#include <list>
#include <sstream>

#include "DirectionalLight.hpp"
#include "Exception.hpp"
#include "Math.hpp"
#include "Scene.hpp"

RT::DirectionalLight::DirectionalLight(Math::Matrix<4, 4> const & transformation, RT::Color const & color, double angle)
  : _color(color), _angle(angle)
{
  // Check values
  if (_angle < 0.f || _angle >= 90.f)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  
  // Calculate position from tranformation matrix
  _position.d().x() = 1.f;
  _position = (transformation * _position).normalize();
}

RT::DirectionalLight::~DirectionalLight()
{}

RT::Color RT::DirectionalLight::preview(RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection) const
{
  // If no ambient light, stop
  if (_color == 0.f || intersection.material.color == 0.f || intersection.material.light.diffuse == 0.f)
    return RT::Color(0.f);

  RT::Ray	light;
  
  // Set light ray from intersection to light origin
  light.d() = _position.d() * -1.f;
  
  // Calculate normal cosinus with light ray
  double	diffuse = std::fmax(RT::Ray::cos(intersection.normal, light), 0.f);
  if (diffuse == 0.f)
    return RT::Color(0.f);

  return intersection.material.color * intersection.material.light.diffuse * diffuse * _color;
}

RT::Color RT::DirectionalLight::render(RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection) const
{
  if (intersection.material.light.diffuse == 0.f && intersection.material.light.specular == 0.f)
    return RT::Color(0.f);

  // Inverse normal if necessary
  bool		inside = false;
  RT::Ray	n = intersection.normal;
  if (RT::Ray::cos(ray, intersection.normal) > 0)
  {
    n.d() *= -1.f;
    inside = true;
  }
  
  std::list<RT::Ray>	rays;
  RT::Ray		r;
  
  r.p() = n.p() + n.d() * Math::Shift;
  
  if (intersection.material.light.quality <= 1 || _angle == 0)
  {
    r.d() = _position.d() * -1.f;
    rays.push_back(r.normalize());
  }
  else
  {
    double  ry, rz;

    // Calculate inverse rotation angles of light
    ry = -std::asin(-_position.d().z());
    if (_position.d().x() != 0 || _position.d().y() != 0)
      rz = -_position.d().y() > 0 ?
      +std::acos(-_position.d().x() / std::sqrt(_position.d().x() * _position.d().x() + _position.d().y() * _position.d().y())) :
      -std::acos(-_position.d().x() / std::sqrt(_position.d().x() * _position.d().x() + _position.d().y() * _position.d().y()));
    else
      rz = 0;

    // Rotation matrix to get ray to light inverse direction
    Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz));

    for (double a = Math::Random::rand(1.f / (intersection.material.light.quality + 1)); a < 1.f; a += 1.f / (intersection.material.light.quality + 1))
      for (double b = Math::Random::rand((2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f)); b < 2.f * Math::Pi; b += (2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f))
      {
	r.d().x() = 1.f / std::tan(Math::Utils::DegToRad(_angle));
	r.d().y() = a * std::cos(b);
	r.d().z() = a * std::sin(b);
	r.d() = matrix * r.d();

	rays.push_back(r.normalize());
      }
  }

  // Calculate reflection ray
  r.d() = n.p() - ray.p() - n.d() * 2.f * (n.d().x() * (n.p().x() - ray.p().x()) + n.d().y() * (n.p().y() - ray.p().y()) + n.d().z() * (n.p().z() - ray.p().z())) / (n.d().x() * n.d().x() + n.d().y() * n.d().y() + n.d().z() * n.d().z());
  
  // Render generated rays
  RT::Color	diffuse, specular;
  for (std::list<RT::Ray>::const_iterator it = rays.begin(); it != rays.end(); it++)
  {
    std::list<RT::Intersection>	intersect = scene->tree()->render((*it));
    RT::Color			light = RT::Color(_color);
    
    // Render light
    while (!intersect.empty() && intersect.front().distance < 0.f)
      intersect.pop_front();
    while (!intersect.empty() && light != RT::Color(0.f))
    {
      light *= intersect.front().material.color * intersect.front().material.transparency.intensity;
      intersect.pop_front();
    }

    // Apply light to diffuse component
    double	cos_d = RT::Ray::cos(n, *it);
    diffuse += light * (cos_d > 0.f ? cos_d : -cos_d);

    // Apply light to specular component
    if (inside == false)
    {
      double	cos_s = RT::Ray::cos(r, *it);
      specular += light * std::pow((cos_s > 0.f ? cos_s : 0.f), intersection.material.light.shininess);
    }
  }

  return diffuse / (double)rays.size() * intersection.material.color * intersection.material.light.diffuse * (1.f - intersection.material.transparency.intensity) * (1.f - intersection.material.reflection.intensity)
    + specular / (double)rays.size() * intersection.material.light.specular;
}

std::string		RT::DirectionalLight::dump() const
{
  std::stringstream	stream;
  double		ry, rz;

  // Calculate rotation angles of light
  ry = +std::asin(-_position.d().z());
  if (_position.d().x() != 0 || _position.d().y() != 0)
    rz = _position.d().y() > 0 ?
    +std::acos(-_position.d().x() / std::sqrt(_position.d().x() * _position.d().x() + _position.d().y() * _position.d().y())) :
    -std::acos(-_position.d().x() / std::sqrt(_position.d().x() * _position.d().x() + _position.d().y() * _position.d().y()));
  else
    rz = 0;

  stream << "transformation(" << Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz)).dump() << ");directional_light(" << _color.dump() << ", " << _angle << ");end();";

  return stream.str();
}