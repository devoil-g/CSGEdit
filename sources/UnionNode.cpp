#include <map>

#include "UnionNode.hpp"

RT::UnionNode::UnionNode()
{}

RT::UnionNode::~UnionNode()
{}

std::list<RT::Intersection>	RT::UnionNode::renderChildren(Math::Ray const & ray) const
{
  std::list<std::list<RT::Intersection> > intersect_list;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(ray);

    // Atribute intersections to children
    for (std::list<RT::Intersection>::iterator it_node = node.begin(); it_node != node.end(); it_node++)
      it_node->node = *it;

    intersect_list.push_back(node);
  }

  std::map<RT::AbstractTree const *, bool>  inside;
  std::list<RT::Intersection>		    intersect, result;
  unsigned int				    state = 0;

  // Merge all intersections
  for (std::list<std::list<RT::Intersection> >::iterator iter = intersect_list.begin(); iter != intersect_list.end(); iter++)
    intersect.merge(*iter);

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

std::string	RT::UnionNode::dump() const
{
  std::stringstream stream;

  stream << "union(){";

  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    if (it != _children.begin())
      stream << ", ";
    stream << (*it)->dump();
  }

  stream << "}";

  return stream.str();
}