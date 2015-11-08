#include "Material.hpp"
#include "Exception.hpp"

std::map<std::string, RT::Material> RT::Material::_material;

RT::Material::Material()
  : color(RT::Color(1.f)), ambient(RT::Color(1.f)), diffuse(RT::Color(1.f)), specular(RT::Color(1.f)), shine(1.f), reflection(0.f), refraction(1.f), transparency(0.f)
{}

RT::Material::~Material()
{}

RT::Material const &  RT::Material::getMaterial(std::string const & name)
{
  // Check if material exist
  if (_material.find(name) != _material.end())
    return _material[name];
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}

void  RT::Material::setMaterial(std::string const & name, RT::Material const & material)
{
  _material[name] = material;
}

void  RT::Material::initialize()
{
  // Set default materials here
}

RT::Material  RT::Material::operator*(RT::Material const & material) const
{
  RT::Material	result;

  // Multiply two materials
  result.color = this->color * material.color;
  result.ambient = this->ambient * material.ambient;
  result.diffuse = this->diffuse * material.diffuse;
  result.specular = this->specular * material.specular;
  result.shine = this->shine * material.shine;
  result.reflection = 1.f - ((1.f - this->reflection) * (1.f - material.reflection));
  result.refraction = this->refraction * material.refraction;
  result.transparency = 1.f - ((1.f - this->transparency) * (1.f - material.transparency));

  return result;
}

RT::Material &	RT::Material::operator*=(RT::Material const & material)
{
  *this = *this * material;
  return *this;
}