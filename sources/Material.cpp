#include <stdexcept>

#include "Material.hpp"
#include "Math.hpp"

std::map<std::string, RT::Material> RT::Material::_material;

RT::Material::Material()
  : color(RT::Color(1.f)), direct(), indirect(), transparency(), reflection()
{}

RT::Material::~Material()
{}

RT::Material const &		RT::Material::getMaterial(std::string const & name)
{
  // Check if material exist
  if (_material.find(name) != _material.end())
    return _material[name];
  else
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
}

void				RT::Material::setMaterial(std::string const & name, RT::Material const & material)
{
  _material[name] = material;
}

void			RT::Material::initialize()
{
  // Set default materials here
}

RT::Material &			RT::Material::operator*=(RT::Material const & material)
{
  // Multiply two materials
  color *= material.color;
  direct *= material.direct;
  indirect *= material.indirect;
  transparency *= material.transparency;
  reflection *= material.reflection;

  return *this;
}

RT::Material			RT::Material::operator*(RT::Material const & material) const
{
  return RT::Material(*this) *= material;
}

RT::Material::Direct &		RT::Material::Direct::operator*=(RT::Material::Direct const & direct)
{
  // Multiply two direct lights
  ambient *= direct.ambient;
  diffuse *= direct.diffuse;
  specular *= direct.specular;
  shininess *= direct.shininess;
  quality = std::min(quality, direct.quality);

  return *this;
}

RT::Material::Direct		RT::Material::Direct::operator*(RT::Material::Direct const & direct) const
{
  return RT::Material::Direct(*this) *= direct;
}

RT::Material::Indirect &	RT::Material::Indirect::operator*=(RT::Material::Indirect const & indirect)
{
  // Multiply two indirect lights
  emission = emission + indirect.emission - emission * indirect.emission;
  quality = std::min(quality, indirect.quality);

  return *this;
}

RT::Material::Indirect		RT::Material::Indirect::operator*(RT::Material::Indirect const & indirect) const
{
  return RT::Material::Indirect(*this) *= indirect;
}

RT::Material::Transparency &	RT::Material::Transparency::operator*=(RT::Material::Transparency const & transparency)
{
  // Multiply two transparencies
  intensity = intensity + transparency.intensity - intensity * transparency.intensity;
  refraction *= transparency.refraction;
  glossiness *= transparency.glossiness;
  quality = std::min(quality, transparency.quality);

  return *this;
}

RT::Material::Transparency	RT::Material::Transparency::operator*(RT::Material::Transparency const & transparency) const
{
  return RT::Material::Transparency(*this) *= transparency;
}

RT::Material::Reflection &	RT::Material::Reflection::operator*=(RT::Material::Reflection const & reflection)
{
  // Multiply two reflections
  intensity = intensity + reflection.intensity - intensity * reflection.intensity;
  glossiness *= reflection.glossiness;
  quality = std::min(quality, reflection.quality);

  return *this;
}

RT::Material::Reflection	RT::Material::Reflection::operator*(RT::Material::Reflection const & reflection) const
{
  return RT::Material::Reflection(*this) *= reflection;
}
