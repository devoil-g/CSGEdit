#include "MaterialNode.hpp"

RT::MaterialNode::MaterialNode(RT::Material const & material)
  : _material(material)
{}

RT::MaterialNode::~MaterialNode()
{}

std::list<RT::Intersection> RT::MaterialNode::renderChildren(RT::Ray const & ray) const
{
  if (_children.empty())
    return std::list<RT::Intersection>();

  std::list<RT::Intersection> intersect;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    std::list<RT::Intersection>	node = (*it)->render(ray);

    // Atribute intersections and apply material to children
    for (std::list<RT::Intersection>::iterator it_node = node.begin(); it_node != node.end(); it_node++)
    {
      it_node->node = *it;
      it_node->material = it_node->material * _material;
    }

    intersect.merge(node);
  }

  std::map<RT::AbstractTree const *, bool>  inside;
  std::list<RT::Intersection>		    result;
  unsigned int				    state = 0;

  // Iterate through intersections
  for (std::list<RT::Intersection>::const_iterator iter = intersect.begin(); iter != intersect.end(); iter++)
  {
    // If currently outside, push intersection
    if (state == 0)
      result.push_back(*iter);

    // Increment deepness if getting inside an object, decrement if getting outside
    if (inside[iter->node])
      state--;
    else
      state++;
    inside[iter->node] = !(inside[iter->node]);

    // If currently outside, push intersection
    if (state == 0)
      result.push_back(*iter);
  }

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

  for (std::list<RT::AbstractTree *>::const_iterator it = _children.begin(); it != _children.end(); it++)
    stream << (*it)->dump();

  for (unsigned int i = 0; i < n; i++)
    stream << "end();";

  return stream.str();
}