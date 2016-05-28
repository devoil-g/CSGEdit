#include "DephCsgNode.hpp"

RT::DephCsgNode::DephCsgNode(unsigned int deph)
  : _deph(deph)
{}

RT::DephCsgNode::~DephCsgNode()
{}

std::list<RT::Intersection>	RT::DephCsgNode::render(RT::Ray const & ray, unsigned int deph) const
{
  if (deph < _deph)
    return RT::UnionCsgNode::render(ray, deph);
  else
    return std::list<RT::Intersection>();
}

size_t				RT::DephCsgNode::build(std::vector<RT::OpenCL::Node> & nodes, std::vector<RT::OpenCL::Primitive> & primitives, Math::Matrix<4, 4> const & transformation, RT::Material const & material, unsigned int deph) const
{
  if (deph < _deph)
    return RT::UnionCsgNode::build(nodes, primitives, transformation, material, deph);
  else
  {
    nodes.push_back(RT::OpenCL::Node());
    nodes.back().type = RT::OpenCL::Node::Type::TypeEmpty;

    return nodes.size() - 1;
  }
}
