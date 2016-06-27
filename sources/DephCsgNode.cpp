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
