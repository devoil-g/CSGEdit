#include "Exception.hpp"
#include "Math.hpp"
#include "PointLight.hpp"
#include "Scene.hpp"

RT::PointLight::PointLight(Math::Matrix<4, 4> const & transformation, RT::Color const & color, double radius, double intensity, double angle1, double angle2, unsigned int quality)
  : _color(color), _radius(radius), _intensity(intensity), _angle1(angle1), _angle2(angle2), _quality(quality)
{
  // Check values
  _radius = _radius > 0.f ? _radius : 0.f;
  _intensity = _intensity > 0.f ? _intensity : 0.f;
  _angle1 = _angle1 > 0.f ? _angle1 : 0.f;
  _angle1 = _angle1 < 180.f ? _angle1 : 180.f;
  _angle2 = _angle2 > _angle1 ? _angle2 : _angle1;
  _angle2 = _angle2 < 180.f ? _angle2 : 180.f;
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

RT::PointLight::~PointLight()
{}

RT::Color RT::PointLight::preview(RT::Scene const * scene, Math::Ray const & ray, Math::Ray const & normal, RT::Material const & material) const
{
  // If no ambient light, stop
  if (scene->config.lightDiffuse == 0.f || _color == RT::Color(0.f) || material.color == RT::Color(0.f) || material.diffuse == 0.f)
    return RT::Color(0.f);

  Math::Ray light, n;
  double    diffuse, intensity, angle;

  // Inverse normal if necessary
  n = normal;
  if (Math::Ray::cos(ray, normal) > 0)
    n.d() = Math::Matrix<4, 4>::scale(-1.f) * normal.d();

  // Calculate intensity level
  if (_intensity == 0.f)
    intensity = 1.f;
  else
    intensity = (_intensity * _intensity) / ((_position.px() - n.px()) * (_position.px() - n.px()) + (_position.py() - n.py()) * (_position.py() - n.py()) + (_position.pz() - n.pz()) * (_position.pz() - n.pz()));
  
  // Set light ray from intersection to light origin
  light.dx() = _position.px() - n.px();
  light.dy() = _position.py() - n.py();
  light.dz() = _position.pz() - n.pz();

  // Calculate normal cosinus with light ray
  diffuse = std::fmax(Math::Ray::cos(n, light), 0.f);
  if (diffuse == 0.f)
    return RT::Color(0.f);

  if (_angle1 == 0.f && _angle2 == 0.f)
    return material.color * material.diffuse * intensity * diffuse * _color * scene->config.lightDiffuse;
  else
  {
    angle = Math::Utils::RadToDeg(std::acos(-Math::Ray::cos(light, _position)));

    if (angle <= _angle1)
      return material.color * material.diffuse * intensity * diffuse * _color * scene->config.lightDiffuse;
    else if (_angle2 > _angle1 && angle < _angle2)
      return RT::Color((_angle2 - angle) / (_angle2 - _angle1)) * material.color * material.diffuse * intensity * diffuse * _color * scene->config.lightDiffuse;
    else
      return RT::Color(0.f);
  }
}

RT::Color RT::PointLight::render(RT::Scene const * scene, Math::Ray const & ray, Math::Ray const & normal, RT::Material const & material) const
{
  if ((scene->config.lightDiffuse == RT::Color(0.f) && scene->config.lightSpecular == RT::Color(0.f)) || (material.diffuse == 0.f && material.specular == 0.f) || material.transparency == 1.f || material.reflection == 1.f)
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

  if (_quality <= 1 || _radius == 0.f)
  {
    r.dx() = _position.px() - r.px();
    r.dy() = _position.py() - r.py();
    r.dz() = _position.pz() - r.pz();
    rays.push_back(r);
  }
  else
  {
    // Random rotation matrix
    Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::rotation(Math::Random::rand(180.f), Math::Random::rand(180.f), Math::Random::rand(180.f));

    for (double a = Math::Random::rand(_radius / (_quality + 1)); a < _radius; a += _radius / (_quality + 1))
      for (double b = Math::Random::rand(Math::Pi / (int)(a / _radius * _quality + 1)); b < Math::Pi; b += Math::Pi / (int)(a / _radius * _quality + 1))
	for (double c = Math::Random::rand(2.f * Math::Pi / (std::sin(b) * a / _radius * _quality + 1)); c < 2.f * Math::Pi; c += 2.f * Math::Pi / (std::sin(b) * a / _radius * _quality + 1))
	{
	  Math::Matrix<4, 1>	sphere;

	  sphere(0, 0) = std::cos(b) * a;
	  sphere(1, 0) = std::cos(c) * std::sin(b) * a;
	  sphere(2, 0) = std::sin(c) * std::sin(b) * a;
	  sphere(3, 0) = 1.f;

	  sphere = matrix * sphere;

	  r.dx() = _position.px() + sphere(0, 0) - r.px();
	  r.dy() = _position.py() + sphere(1, 0) - r.py();
	  r.dz() = _position.pz() + sphere(2, 0) - r.pz();
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
    std::list<RT::Intersection>	intersect;
    RT::Color			light = RT::Color(_color);
    double			cos_d = Math::Ray::cos(n, *it);
    double			cos_s = Math::Ray::cos(r, *it);
    double			angle, intensity;

    if ((_angle1 == 0.f && _angle2 == 0.f) || (angle = acos(-Math::Ray::cos(_position, *it))) <= _angle1)
      angle = 1.f;
    else if (_angle2 > _angle1 && angle < _angle2)
      angle = (_angle2 - angle) / (_angle2 - _angle1);
    else
      angle = 0.f;
    
    if (_intensity == 0.f)
      intensity = 1.f;
    else
      intensity = (_intensity * _intensity) / (it->dx() * it->dx() + it->dy() * it->dy() + it->dz() * it->dz());

    if (angle != 0.f)
      intersect = scene->tree->render((*it));

    // Render light
    while (!intersect.empty() && intersect.front().distance < 0.f)
      intersect.pop_front();
    while (!intersect.empty() && intersect.front().distance < 1.f && light != RT::Color(0.f))
    {
      light *= intersect.front().material.color * intersect.front().material.transparency;
      intersect.pop_front();
    }

    // Apply light to diffuse component
    diffuse += light * (cos_d > 0.f ? cos_d : -cos_d) * angle * intensity;

    // Apply light to specular component
    specular += light * pow((cos_s > 0.f ? cos_s : 0.f), material.shine) * angle * intensity;
  }

  return diffuse / (double)rays.size() * scene->config.lightDiffuse * material.color * material.diffuse * (1.f - material.transparency) * (1.f - material.reflection)
    + specular / (double)rays.size() * scene->config.lightSpecular * material.specular * (1.f - material.transparency) * (1.f - material.reflection);
}

std::string		RT::PointLight::dump() const
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

  stream << "transformation(" << (Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz)) * Math::Matrix<4, 4>::translation(_position.px(), _position.py(), _position.pz())).dump() << ");point_light(" << _color.dump() << ", " << _radius << ", " << _intensity << ", " << _angle1 << ", " << _angle2 << ", " << _quality << ");end();";

  return stream.str();
}