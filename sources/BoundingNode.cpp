#include <unordered_map>

#include "BoundingNode.hpp"

RT::BoundingNode::BoundingNode()
{}

RT::BoundingNode::~BoundingNode()
{}

std::list<RT::Intersection>	RT::BoundingNode::renderChildren(RT::Ray const & ray) const
{
  // Stop if no intersection with bounding tree
  if (_children.front()->render(ray).empty())
    return std::list<RT::Intersection>();
 
  std::unordered_map<RT::AbstractTree const *, bool>	inside;
  std::list<RT::Intersection>				intersect, result;
  unsigned int						state = 0;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree *>::const_iterator it = std::next(_children.begin()); it != _children.end(); it++)
    intersect.merge((*it)->render(ray));

  // Iterate through intersections
  for (RT::Intersection const & it : intersect)
  {
    // If currently outside, push intersection
    if (state == 0)
      result.push_back(it);

    // Increment deepness if getting inside an object, decrement if getting outside
    state += inside[it.node] ? -1 : +1;
    inside[it.node] = !(inside[it.node]);

    // If currently outside, push intersection
    if (state == 0)
      result.push_back(it);
  }

  return result;
}

std::string	RT::BoundingNode::dump() const
{
  std::stringstream stream;

  stream << "bounding();";

  for (std::list<RT::AbstractTree *>::const_iterator it = _children.begin(); it != _children.end(); it++)
    stream << (*it)->dump();

  stream << "end();";

  return stream.str();
}