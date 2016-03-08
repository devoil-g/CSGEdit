#include <map>

#include "IntersectionNode.hpp"

RT::IntersectionNode::IntersectionNode()
{}

RT::IntersectionNode::~IntersectionNode()
{}

std::list<RT::Intersection>	RT::IntersectionNode::renderChildren(RT::Ray const & ray) const
{
  std::list<RT::Intersection>			intersect;

  // Iterate through sub-tree to get intersections
  for (RT::AbstractTree const * it : _children)
  {
    std::list<RT::Intersection> node = it->render(ray);

    // Acceleration tweak
    if (node.empty())
      return std::list<RT::Intersection>();
    
    intersect.merge(node);
  }

  std::map<RT::AbstractTree const *, bool>	inside;
  std::list<RT::Intersection>			result;
  unsigned int					state = 0;

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

std::string	RT::IntersectionNode::dump() const
{
  std::stringstream stream;

  stream << "intersection();";

  for (std::list<RT::AbstractTree *>::const_iterator it = _children.begin(); it != _children.end(); it++)
    stream << (*it)->dump();

  stream << "end();";

  return stream.str();
}