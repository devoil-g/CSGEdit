#include <map>

#include "IntersectionNode.hpp"

RT::IntersectionNode::IntersectionNode()
{}

RT::IntersectionNode::~IntersectionNode()
{}

std::list<RT::Intersection>	RT::IntersectionNode::renderChildren(RT::Ray const & ray) const
{
  if (_children.empty())
    return std::list<RT::Intersection>();

  std::list<RT::Intersection>			intersect;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(ray);

    // Acceleration tweak
    if (node.empty())
      return std::list<RT::Intersection>();

    // Atribute intersections to children
    for (std::list<RT::Intersection>::iterator it_node = node.begin(); it_node != node.end(); it_node++)
      it_node->node = *it;

    intersect.merge(node);
  }

  std::map<RT::AbstractTree const *, bool>	inside;
  std::list<RT::Intersection>			result;
  unsigned int					state = 0;

  // Iterate through intersections
  for (std::list<RT::Intersection>::iterator iter = intersect.begin(); iter != intersect.end(); iter++)
  {
    // If inside all children, push itersection
    if (state == _children.size())
      result.push_back(*iter);

    // Increment deepness if getting inside an object, decrement if getting outside
    if (inside[iter->node])
      state--;
    else
      state++;
    inside[iter->node] = !(inside[iter->node]);

    // If inside all children, push itersection
    if (state == _children.size())
      result.push_back(*iter);
  }

  return result;
}

std::string	RT::IntersectionNode::dump() const
{
  std::stringstream stream;

  stream << "intersection();";

  for (std::list<RT::AbstractTree *>::const_iterator it = _children.begin(); it != _children.end(); it++)
    stream << (*it)->dump();

  stream << "end();";

  return stream.str();
}