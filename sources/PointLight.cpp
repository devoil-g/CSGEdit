#include "Exception.hpp"
#include "Math.hpp"
#include "PointLight.hpp"
#include "Scene.hpp"

RT::PointLight::PointLight(Math::Matrix<4, 4> const & transformation, RT::Color const & color, double radius, double intensity, double angle1, double angle2)
  : _color(color), _radius(radius), _intensity(intensity), _angle1(angle1), _angle2(angle2 > angle1 ? angle2 : angle1)
{
  // Check values
  if (_radius < 0.f)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  if (_intensity < 0.f)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  if (_angle1 < 0.f || _angle1 > 180.f)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  if (_angle2 < 0.f || _angle2 > 180.f)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  // Calculate position from tranformation matrix
  _position.d().x() = 1.f;
  _position = (transformation * _position).normalize();
}

RT::PointLight::~PointLight()
{}

RT::Color RT::PointLight::preview(RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection) const
{
  // If no diffuse light, stop
  if (_color == 0.f || intersection.material.color == 0.f || intersection.material.light.diffuse == 0.f)
    return RT::Color(0.f);

  RT::Ray	light;
  
  // Calculate intensity level
  double	intensity;
  if (_intensity == 0.f)
    intensity = 1.f;
  else
    intensity = (_intensity * _intensity) / ((_position.p().x() - intersection.normal.p().x()) * (_position.p().x() - intersection.normal.p().x()) + (_position.p().y() - intersection.normal.p().y()) * (_position.p().y() - intersection.normal.p().y()) + (_position.p().z() - intersection.normal.p().z()) * (_position.p().z() - intersection.normal.p().z()));
  
  // Set light ray from intersection to light origin
  light.d() = _position.p() - intersection.normal.p();
  
  // Calculate normal cosinus with light ray
  double	diffuse = std::fmax(RT::Ray::cos(intersection.normal.d(), light.d()), 0.f);
  if (diffuse == 0.f)
    return RT::Color(0.f);

  if (_angle1 == 0.f && _angle2 == 0.f)
    return intersection.material.color * intersection.material.light.diffuse * intensity * diffuse * _color;
  else
  {
    double	angle = Math::Utils::RadToDeg(std::acos(-RT::Ray::cos(light.d(), _position.d())));

    if (angle < _angle1)
      return intersection.material.color * intersection.material.light.diffuse * intensity * diffuse * _color;
    else if (angle < _angle2)
      return RT::Color((_angle2 - angle) / (_angle2 - _angle1)) * intersection.material.color * intersection.material.light.diffuse * intensity * diffuse * _color;
    else
      return RT::Color(0.f);
  }
}

RT::Color RT::PointLight::render(RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection) const
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
  RT::Ray		r;
  RT::Color		diffuse, specular;

  r.p() = intersection.normal.p() + n * Math::Shift;
  
  if (intersection.material.light.quality <= 1 || _radius == 0.f)
  {
    r.d() = _position.p() - r.p();
    rays.push_back(r);
  }
  else
  {
    // Random rotation matrix
    Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::rotation(Math::Random::rand(180.f), Math::Random::rand(180.f), Math::Random::rand(180.f));

    for (double a = Math::Random::rand(_radius / (intersection.material.light.quality + 1)); a < _radius; a += _radius / (intersection.material.light.quality + 1))
      for (double b = Math::Random::rand(Math::Pi / (int)(a / _radius * intersection.material.light.quality + 1)); b < Math::Pi; b += Math::Pi / (int)(a / _radius * intersection.material.light.quality + 1))
	for (double c = Math::Random::rand(2.f * Math::Pi / (std::sin(b) * a / _radius * intersection.material.light.quality + 1)); c < 2.f * Math::Pi; c += 2.f * Math::Pi / (std::sin(b) * a / _radius * intersection.material.light.quality + 1))
	{
	  Math::Vector<4>	sphere;

	  sphere(0) = std::cos(b) * a;
	  sphere(1) = std::cos(c) * std::sin(b) * a;
	  sphere(2) = std::sin(c) * std::sin(b) * a;
	  
	  r.d() = _position.p() + matrix * sphere - r.p();
	  rays.push_back(r);
	}
  }

  // Calculate reflection ray
  r.d() = intersection.normal.p() - ray.p() - n * 2.f * (n.x() * (intersection.normal.p().x() - ray.p().x()) + n.y() * (intersection.normal.p().y() - ray.p().y()) + n.z() * (intersection.normal.p().z() - ray.p().z())) / (n.x() * n.x() + n.y() * n.y() + n.z() * n.z());
  
  // Render generated rays
  for (std::list<RT::Ray>::const_iterator it = rays.begin(); it != rays.end(); it++)
  {
    std::list<RT::Intersection>	intersect;
    RT::Color			light = RT::Color(_color);
    double			angle, intensity;

    if ((_angle1 == 0.f && _angle2 == 0.f) || (angle = acos(-RT::Ray::cos(_position.d(), it->d()))) <= _angle1)
      angle = 1.f;
    else if (_angle2 > _angle1 && angle < _angle2)
      angle = (_angle2 - angle) / (_angle2 - _angle1);
    else
      angle = 0.f;
    
    if (_intensity == 0.f)
      intensity = 1.f;
    else
      intensity = (_intensity * _intensity) / (it->d().x() * it->d().x() + it->d().y() * it->d().y() + it->d().z() * it->d().z());

    if (angle != 0.f)
      intersect = scene->tree()->render((*it));

    // Render light
    while (!intersect.empty() && intersect.front().distance < 0.f)
      intersect.pop_front();
    while (!intersect.empty() && intersect.front().distance < 1.f && light != RT::Color(0.f))
    {
      light *= intersect.front().material.color * intersect.front().material.transparency.intensity;
      intersect.pop_front();
    }

    // Apply light to diffuse component
    double	cos_d = RT::Ray::cos(n, it->d());
    diffuse += light * (cos_d > 0.f ? cos_d : -cos_d) * angle * intensity;

    // Apply light to specular component
    if (inside == false)
    {
      double	cos_s = RT::Ray::cos(r.d(), it->d());
      specular += light * pow((cos_s > 0.f ? cos_s : 0.f), intersection.material.light.shininess) * angle * intensity;
    }
  }

  return diffuse / (double)rays.size() * intersection.material.color * intersection.material.light.diffuse * (1.f - intersection.material.transparency.intensity) * (1.f - intersection.material.reflection.intensity)
    + specular / (double)rays.size() * intersection.material.light.specular;
}

std::string		RT::PointLight::dump() const
{
  std::stringstream	stream;
  Math::Matrix<4, 4>	transformation;
  double		ry, rz;

  // Calculate rotation angles of light
  ry = +std::asin(-_position.d().z());
  if (_position.d().x() != 0 || _position.d().y() != 0)
    rz = _position.d().y() > 0 ?
    +std::acos(-_position.d().x() / std::sqrt(_position.d().x() * _position.d().x() + _position.d().y() * _position.d().y())) :
    -std::acos(-_position.d().x() / std::sqrt(_position.d().x() * _position.d().x() + _position.d().y() * _position.d().y()));
  else
    rz = 0;

  stream << "transformation(" << (Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz)) * Math::Matrix<4, 4>::translation(_position.p().x(), _position.p().y(), _position.p().z())).dump() << ");point_light(" << _color.dump() << ", " << _radius << ", " << _intensity << ", " << _angle1 << ", " << _angle2 << ");end();";

  return stream.str();
}