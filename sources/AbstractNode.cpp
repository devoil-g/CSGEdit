#include "AbstractNode.hpp"
#include "Exception.hpp"

RT::AbstractNode::AbstractNode()
{}

RT::AbstractNode::~AbstractNode()
{
  while (!_children.empty())
  {
    delete _children.front();
    _children.pop_front();
  }
}

std::list<RT::Intersection>	RT::AbstractNode::render(RT::Ray const & ray) const
{
  if (_children.empty())
    return std::list<RT::Intersection>();

  std::list<RT::Intersection>	result = renderChildren(ray);

  // Attribute top node
  for (RT::Intersection & it : result)
    it.node = this;

  return result;
}

void	RT::AbstractNode::push(RT::AbstractTree * node)
{
  // Push sub-tree in list if not nullptr
  if (node)
    _children.push_back(node);
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}
