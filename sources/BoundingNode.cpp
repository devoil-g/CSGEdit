#include <map>

#include "BoundingNode.hpp"

RT::BoundingNode::BoundingNode()
{}

RT::BoundingNode::~BoundingNode()
{}

std::list<RT::Intersection>	RT::BoundingNode::renderChildren(Math::Ray const & ray) const
{
  std::map<RT::AbstractTree const *, bool>  inside;
  std::list<RT::Intersection>		    intersect, result;
  unsigned int				    state = 0;

  // Stop if no intersection with bounding tree
  if (_children.empty() || _children.front()->render(ray).empty())
    return std::list<RT::Intersection>();
  
  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree const *>::const_iterator it = std::next(_children.begin()); it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(ray);

    // Atribute intersections to children
    for (std::list<RT::Intersection>::iterator it_node = node.begin(); it_node != node.end(); it_node++)
      it_node->node = *it;

    intersect.merge(node);
  }

  // Set all positions to 'outside' (false)
  for (std::list<RT::Intersection>::iterator iter = intersect.begin(); iter != intersect.end(); iter++)
    inside[iter->node] = false;

  // Iterate through intersections
  for (std::list<RT::Intersection>::iterator iter = intersect.begin(); iter != intersect.end(); iter++)
  {
    // If currently outside, push intersection
    if (state == 0)
      result.push_back(*iter);

    // Increment deepness if getting inside an object, decrement if getting outside
    if (inside[iter->node])
      state--;
    else
      state++;
    inside[iter->node] = !(inside[iter->node]);

    // If currently outside, push intersection
    if (state == 0)
      result.push_back(*iter);
  }

  return result;
}

std::string	RT::BoundingNode::dump() const
{
  std::stringstream stream;

  stream << "bounding(){";

  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    if (it != _children.begin())
      stream << ", ";
    stream << (*it)->dump();
  }

  stream << "}";

  return stream.str();
}