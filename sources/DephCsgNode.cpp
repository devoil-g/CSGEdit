#include "DephCsgNode.hpp"

RT::DephCsgNode::DephCsgNode(unsigned int deph)
  : _deph(deph)
{}

RT::DephCsgNode::~DephCsgNode()
{}

std::list<RT::Intersection>	RT::DephCsgNode::renderChildren(RT::Ray const & ray, unsigned int deph) const
{
  if (deph < _deph)
    return RT::UnionCsgNode::renderChildren(ray, deph);
  else
    return std::list<RT::Intersection>();
}
