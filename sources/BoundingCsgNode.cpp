#include "BoundingCsgNode.hpp"

RT::BoundingCsgNode::BoundingCsgNode()
  : _bound(nullptr)
{}

RT::BoundingCsgNode::~BoundingCsgNode()
{
  delete _bound;
}

std::list<RT::Intersection>	RT::BoundingCsgNode::render(RT::Ray const & ray, unsigned int deph) const
{
  // Stop if no intersection with bounding tree
  if (_bound->render(ray, deph).empty())
    return std::list<RT::Intersection>();
  else
    return RT::UnionCsgNode::render(ray, deph);
}

void				RT::BoundingCsgNode::push(RT::AbstractCsgTree * node)
{
  // Push sub-tree in list if not nullptr
  if (node && _bound == nullptr)
    _bound = node;
  else
    RT::AbstractCsgNode::push(node);
}

size_t				RT::BoundingCsgNode::build(std::vector<RT::OpenCL::Node> & nodes, std::vector<RT::OpenCL::Primitive> & primitives, Math::Matrix<4, 4> const & transformation, RT::Material const & material, unsigned int deph) const
{
  return RT::UnionCsgNode::build(nodes, primitives, transformation, material, deph);
}
