#include "AbstractNode.hpp"
#include "Exception.hpp"

RT::AbstractNode::AbstractNode()
{}

RT::AbstractNode::AbstractNode(Math::Matrix<4, 4> const & transformation)
  : AbstractTree(transformation)
{}

RT::AbstractNode::~AbstractNode()
{
  while (!_children.empty())
  {
    delete _children.front();
    _children.pop_front();
  }
}

void	RT::AbstractNode::push(RT::AbstractTree const * node)
{
  // Push sub-tree in list if not nullptr
  if (node)
    _children.push_back(node);
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}
