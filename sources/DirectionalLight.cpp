#include <list>
#include <sstream>

#include "DirectionalLight.hpp"
#include "Math.hpp"
#include "Scene.hpp"

RT::DirectionalLight::DirectionalLight(Math::Matrix<4, 4> const & transformation, RT::Color const & color, double angle, unsigned int quality)
  : _color(color), _angle(angle), _quality(quality)
{
  // Check values
  _angle = _angle > 0.f ? _angle : 0.f;
  _quality = _quality > 1 ? _quality : 1;

  // Calculate position from tranformation matrix
  _position.px() = 0.f;
  _position.py() = 0.f;
  _position.pz() = 0.f;
  _position.dx() = 1.f;
  _position.dy() = 0.f;
  _position.dz() = 0.f;
  _position = (transformation * _position).normalize();
}

RT::DirectionalLight::~DirectionalLight()
{}

RT::Color RT::DirectionalLight::preview(RT::Scene const * scene, Math::Ray const & ray, Math::Ray const & normal, RT::Material const & material) const
{
  // If no ambient light, stop
  if (scene->config().lightDiffuse == 0.f || _color == RT::Color(0.f) || material.color == RT::Color(0.f) || material.diffuse == 0.f)
    return RT::Color(0.f);

  Math::Ray light, n;
  double    diffuse;

  // Inverse normal if necessary
  n = normal;
  if (Math::Ray::cos(ray, normal) > 0)
    n.d() = Math::Matrix<4, 4>::scale(-1.f) * normal.d();

  // Set light ray from intersection to light origin
  light.dx() = -_position.dx();
  light.dy() = -_position.dy();
  light.dz() = -_position.dz();

  // Calculate normal cosinus with light ray
  diffuse = std::fmax(Math::Ray::cos(n, light), 0.f);
  if (diffuse == 0.f)
    return RT::Color(0.f);

  return material.color * material.diffuse * diffuse * _color * scene->config().lightDiffuse;
}

RT::Color RT::DirectionalLight::render(RT::Scene const * scene, Math::Ray const & ray, Math::Ray const & normal, RT::Material const & material) const
{
  if ((scene->config().lightDiffuse == RT::Color(0.f) && scene->config().lightSpecular == RT::Color(0.f)) || (material.diffuse == 0.f && material.specular == 0.f) || material.transparency == 1.f || material.reflection == 1.f)
    return RT::Color(0.f);

  Math::Ray n;

  // Inverse normal if necessary
  n = normal;
  if (Math::Ray::cos(ray, normal) > 0)
    n.d() = Math::Matrix<4, 4>::scale(-1.f) * normal.d();
  
  std::list<Math::Ray>	rays;
  Math::Ray		r;
  RT::Color		diffuse, specular;

  r.px() = n.px() + n.dx() * Math::Shift;
  r.py() = n.py() + n.dy() * Math::Shift;
  r.pz() = n.pz() + n.dz() * Math::Shift;

  if (_quality <= 1 || _angle == 0)
  {
    r.dx() = -_position.dx();
    r.dy() = -_position.dy();
    r.dz() = -_position.dz();
    rays.push_back(r);
  }
  else
  {
    double  ry, rz;

    // Calculate inverse rotation angles of light
    ry = -std::asin(-_position.dz());
    if (_position.dx() != 0 || _position.dy() != 0)
      rz = -_position.dy() > 0 ?
      +std::acos(-_position.dx() / std::sqrt(_position.dx() * _position.dx() + _position.dy() * _position.dy())) :
      -std::acos(-_position.dx() / std::sqrt(_position.dx() * _position.dx() + _position.dy() * _position.dy()));
    else
      rz = 0;

    // Rotation matrix to get ray to light inverse direction
    Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz));

    for (double a = Math::Random::rand(1.f / (_quality + 1)); a < 1.f; a += 1.f / (_quality + 1))
      for (double b = Math::Random::rand((2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f)); b < 2.f * Math::Pi; b += (2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f))
      {
	r.dx() = 1.f / std::tan(Math::Utils::DegToRad(_angle));
	r.dy() = a * std::cos(b);
	r.dz() = a * std::sin(b);
	r.d() = matrix * r.d();
	rays.push_back(r);
      }
  }

  // Calculate reflection ray
  double k = -(n.dx() * (n.px() - ray.px()) + n.dy() * (n.py() - ray.py()) + n.dz() * (n.pz() - ray.pz())) / (n.dx() * n.dx() + n.dy() * n.dy() + n.dz() * n.dz());
  r.dx() = ray.px() + 2.f * (n.px() + n.dx() * k - ray.px()) - n.px();
  r.dy() = ray.py() + 2.f * (n.py() + n.dy() * k - ray.py()) - n.py();
  r.dz() = ray.pz() + 2.f * (n.pz() + n.dz() * k - ray.pz()) - n.pz();

  // Render generated rays
  for (std::list<Math::Ray>::const_iterator it = rays.begin(); it != rays.end(); it++)
  {
    std::list<RT::Intersection>	intersect = scene->tree()->render((*it).normalize());
    RT::Color			light = RT::Color(_color);
    double			cos_d = Math::Ray::cos(n, *it);
    double			cos_s = Math::Ray::cos(r, *it);

    // Render light
    while (!intersect.empty() && intersect.front().distance < 0.f)
      intersect.pop_front();
    while (!intersect.empty() && light != RT::Color(0.f))
    {
      light *= intersect.front().material.color * intersect.front().material.transparency;
      intersect.pop_front();
    }

    // Apply light to diffuse component
    diffuse += light * (cos_d > 0.f ? cos_d : -cos_d);

    // Apply light to specular component
    specular += light * std::pow((cos_s > 0.f ? cos_s : 0.f), material.shine);
  }

  return diffuse / (double)rays.size() * scene->config().lightDiffuse * material.color * material.diffuse * (1.f - material.transparency) * (1.f - material.reflection)
    + specular / (double)rays.size() * scene->config().lightSpecular * material.specular * (1.f - material.transparency) * (1.f - material.reflection);
}

std::string		RT::DirectionalLight::dump() const
{
  std::stringstream	stream;
  Math::Matrix<4, 4>	transformation;
  double		ry, rz;

  // Calculate rotation angles of light
  ry = +std::asin(-_position.dz());
  if (_position.dx() != 0 || _position.dy() != 0)
    rz = _position.dy() > 0 ?
    +std::acos(-_position.dx() / std::sqrt(_position.dx() * _position.dx() + _position.dy() * _position.dy())) :
    -std::acos(-_position.dx() / std::sqrt(_position.dx() * _position.dx() + _position.dy() * _position.dy()));
  else
    rz = 0;

  stream << "transformation(" << Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz)).dump() << ");directional_light(" << _color.dump() << ", " << _angle << ", " << _quality << ");end();";

  return stream.str();
}