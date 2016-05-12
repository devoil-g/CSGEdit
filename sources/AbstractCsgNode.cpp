#include <stdexcept>

#include "AbstractCsgNode.hpp"

RT::AbstractCsgNode::AbstractCsgNode()
{}

RT::AbstractCsgNode::~AbstractCsgNode()
{
  for (RT::AbstractCsgTree * it : _children)
    delete it;
}

void	RT::AbstractCsgNode::push(RT::AbstractCsgTree * node)
{
  // Push sub-tree in list if not nullptr
  if (node)
    _children.push_back(node);
  else
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
}

void	RT::AbstractCsgNode::pop()
{
  if (!_children.empty())
  {
    delete _children.back();
    _children.pop_back();
  }
  else
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
}
