#include "MaterialCsgNode.hpp"

RT::MaterialCsgNode::MaterialCsgNode(RT::Material const & material)
  : _material(material)
{}

RT::MaterialCsgNode::~MaterialCsgNode()
{}

std::list<RT::Intersection>	RT::MaterialCsgNode::render(RT::Ray const & ray, unsigned int deph) const
{
  std::list<RT::Intersection>	result = RT::UnionCsgNode::render(ray, deph);

  // Atribute intersections and apply material to children
  for (RT::Intersection & it : result)
    it.material *= _material;

  return result;
}
