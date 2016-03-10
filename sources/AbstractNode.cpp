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
#ifdef _DEBUG
  if (_children.empty())
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

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
