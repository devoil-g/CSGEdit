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

size_t				RT::MaterialCsgNode::build(std::vector<RT::OpenCL::Node> & nodes, std::vector<RT::OpenCL::Primitive> & primitives, Math::Matrix<4, 4> const & transformation, RT::Material const & material, unsigned int deph) const
{
  return RT::UnionCsgNode::build(nodes, primitives, transformation, material * _material, deph);
}
