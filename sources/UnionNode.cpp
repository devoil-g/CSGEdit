#include <map>

#include "UnionNode.hpp"

RT::UnionNode::UnionNode()
{}

RT::UnionNode::~UnionNode()
{}

std::list<RT::Intersection>	RT::UnionNode::renderChildren(RT::Ray const & ray) const
{
  std::list<RT::Intersection>			intersect, result;
  std::map<RT::AbstractTree const *, bool>	inside;
  unsigned int					state = 0;

  // Iterate through sub-tree to get intersections
  for (RT::AbstractTree const * it : _children)
    intersect.merge(it->render(ray));

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

std::string	RT::UnionNode::dump() const
{
  std::stringstream stream;

  stream << "union();";

  for (RT::AbstractTree const * it : _children)
    stream << it->dump();

  stream << "end();";

  return stream.str();
}