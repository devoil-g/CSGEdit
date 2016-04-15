#include "AbstractLightNode.hpp"
#include "Exception.hpp"

RT::AbstractLightNode::AbstractLightNode()
{}

RT::AbstractLightNode::~AbstractLightNode()
{
  for (RT::AbstractLightTree * it : _children)
    delete it;
}

void	RT::AbstractLightNode::push(RT::AbstractLightTree * node)
{
  // Push sub-tree in list if not nullptr
  if (node)
    _children.push_back(node);
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}

void	RT::AbstractLightNode::pop()
{
  if (!_children.empty())
  {
    delete _children.back();
    _children.pop_back();
  }
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}

bool	RT::AbstractLightNode::empty() const
{
  return _children.empty();
}
