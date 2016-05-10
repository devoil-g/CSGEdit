#include <exception>

#include "AbstractCsgNode.hpp"

RT::AbstractCsgNode::AbstractCsgNode()
{}

RT::AbstractCsgNode::~AbstractCsgNode()
{
  for (RT::AbstractCsgTree * it : _children)
    delete it;
}

std::list<RT::Intersection>	RT::AbstractCsgNode::render(RT::Ray const & ray, unsigned int deph) const
{
#ifdef _DEBUG
  if (_children.empty())
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
#endif

  std::list<RT::Intersection>	result = renderChildren(ray, deph);

  // Attribute top node
  for (RT::Intersection & it : result)
    it.node = this;

  return result;
}

void	RT::AbstractCsgNode::push(RT::AbstractCsgTree * node)
{
  // Push sub-tree in list if not nullptr
  if (node)
    _children.push_back(node);
  else
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
}

void	RT::AbstractCsgNode::pop()
{
  if (!_children.empty())
  {
    delete _children.back();
    _children.pop_back();
  }
  else
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
}
