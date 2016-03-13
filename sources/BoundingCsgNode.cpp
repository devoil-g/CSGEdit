#include <unordered_map>

#include "BoundingCsgNode.hpp"

RT::BoundingCsgNode::BoundingCsgNode()
{}

RT::BoundingCsgNode::~BoundingCsgNode()
{}

std::list<RT::Intersection>	RT::BoundingCsgNode::renderChildren(RT::Ray const & ray, unsigned int deph) const
{
  // Stop if no intersection with bounding tree
  if (_children.front()->render(ray, deph).empty())
    return std::list<RT::Intersection>();
 
  std::unordered_map<RT::AbstractCsgTree const *, bool>	inside;
  std::list<RT::Intersection>				intersect, result;
  unsigned int						state = 0;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractCsgTree *>::const_iterator it = std::next(_children.begin()); it != _children.end(); it++)
    intersect.merge((*it)->render(ray, deph));

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
