#include <unordered_map>

#include "UnionCsgNode.hpp"

RT::UnionCsgNode::UnionCsgNode()
{}

RT::UnionCsgNode::~UnionCsgNode()
{}

std::list<RT::Intersection>	RT::UnionCsgNode::renderChildren(RT::Ray const & ray, unsigned int deph) const
{
  std::list<RT::Intersection>				intersect, result;
  std::unordered_map<RT::AbstractCsgTree const *, bool>	inside;
  unsigned int						state = 0;

  // Iterate through sub-tree to get intersections
  for (RT::AbstractCsgTree const * it : _children)
    intersect.merge(it->render(ray, deph));

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
