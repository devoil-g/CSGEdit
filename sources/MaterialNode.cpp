#include <unordered_map>

#include "MaterialNode.hpp"

RT::MaterialNode::MaterialNode(RT::Material const & material)
  : _material(material)
{}

RT::MaterialNode::~MaterialNode()
{}

std::list<RT::Intersection> RT::MaterialNode::renderChildren(RT::Ray const & ray) const
{
  std::list<RT::Intersection> intersect;

  // Iterate through sub-tree to get intersections
  for (RT::AbstractTree const * it : _children)
    intersect.merge(it->render(ray));

  std::unordered_map<RT::AbstractTree const *, bool>	inside;
  std::list<RT::Intersection>				result;
  unsigned int						state = 0;

  // Iterate through intersections
  for (RT::Intersection const & it : intersect)
  {
    // If currently outside, push intersection
    if (state == 0)
      result.push_back(it);

    // Increment deepness if getting inside an object, decrement if getting outside
    state += inside[it.node] ? -1 : +1;
    inside[it.node] = !(inside[it.node]);

    // If currently outside, push intersection
    if (state == 0)
      result.push_back(it);
  }

  // Atribute intersections and apply material to children
  for (RT::Intersection & it : result)
    it.material *= _material;

  return result;
}

std::string	RT::MaterialNode::dump() const
{
  std::stringstream stream;
  unsigned int	    n = 0;

  // Search for non-default materials
  if (_material.color != 1.f)
  {
    stream << "color(" << _material.color.dump() << ");";
    n++;
  }
  if (_material.light.ambient != 1.f || _material.light.diffuse != 1.f || _material.light.specular != 1.f || _material.light.shininess != 1.f || _material.light.quality != RT::Config::Material::Quality)
  {
    stream << "light(" << _material.light.ambient.dump() << ", " << _material.light.diffuse.dump() << ", " << _material.light.specular.dump() << ", " << _material.light.shininess << ", " << _material.light.quality << ");";
    n++;
  }
  if (_material.transparency.intensity != 0.f || _material.transparency.refraction != 1.f || _material.transparency.glossiness != 0.f || _material.transparency.quality != RT::Config::Material::Quality)
  {
    stream << "transparency(" << _material.transparency.intensity << ", " << _material.transparency.refraction << ", " << _material.transparency.glossiness << ", " << _material.transparency.quality << ");";
    n++;
  }
  if (_material.reflection.intensity != 0.f || _material.reflection.glossiness != 0.f || _material.reflection.quality != RT::Config::Material::Quality)
  {
    stream << "reflection(" << _material.reflection.intensity << ", " << _material.reflection.glossiness << ", " << _material.reflection.quality << ");";
    n++;
  }

  // If no material, force an union
  if (n == 0)
  {
    stream << "union();";
    n++;
  }

  for (RT::AbstractTree const * it : _children)
    stream << it->dump();

  for (unsigned int i = 0; i < n; i++)
    stream << "end();";

  return stream.str();
}