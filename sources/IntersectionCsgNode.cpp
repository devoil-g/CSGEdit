#include <unordered_map>

#include "IntersectionCsgNode.hpp"

RT::IntersectionCsgNode::IntersectionCsgNode()
{}

RT::IntersectionCsgNode::~IntersectionCsgNode()
{}

std::list<RT::Intersection>	RT::IntersectionCsgNode::renderChildren(RT::Ray const & ray, unsigned int deph) const
{
  std::list<RT::Intersection>	intersect;

  // Iterate through sub-tree to get intersections
  for (RT::AbstractCsgTree const * it : _children)
  {
    std::list<RT::Intersection> node = it->render(ray, deph);

    // Acceleration tweak
    if (node.empty())
      return std::list<RT::Intersection>();
    
    intersect.merge(node);
  }

  std::unordered_map<RT::AbstractCsgTree const *, bool>	inside;
  std::list<RT::Intersection>				result;
  unsigned int						state = 0;

  // Iterate through intersections
  for (RT::Intersection const & it : intersect)
  {
    // If inside all children, push intersection
    if (state == _children.size())
      result.push_back(it);

    // Increment deepness if getting inside an object, decrement if getting outside
    state += inside[it.node] ? -1 : +1;
    inside[it.node] = !(inside[it.node]);

    // If inside all children, push intersection
    if (state == _children.size())
      result.push_back(it);
  }

  return result;
}
