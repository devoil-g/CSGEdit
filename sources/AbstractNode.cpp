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

std::list<RT::Intersection>	RT::AbstractNode::render(Math::Ray const & ray) const
{
  std::list<RT::Intersection>	result = renderChildren(ray);

  // Attribute top node
  for (std::list<RT::Intersection>::iterator it = result.begin(); it != result.end(); it++)
    (*it).node = this;

  return result;
}

void	RT::AbstractNode::push(RT::AbstractTree const * node)
{
  // Push sub-tree in list if not nullptr
  if (node)
    _children.push_back(node);
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}
