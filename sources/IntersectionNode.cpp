#include <map>

#include "IntersectionNode.hpp"

RT::IntersectionNode::IntersectionNode()
{}

RT::IntersectionNode::IntersectionNode(Math::Matrix<4, 4> const & transformation)
  : AbstractNode(transformation)
{}

RT::IntersectionNode::~IntersectionNode()
{}

std::list<RT::Intersection>	RT::IntersectionNode::renderTree(Math::Ray const & ray) const
{
  std::list<std::list<RT::Intersection> > intersect_list;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(ray);

    // Atribute intersections to children
    for (std::list<RT::Intersection>::iterator it_node = node.begin(); it_node != node.end(); it_node++)
      it_node->node = *it;

    // Acceleration tweak
    if (node.empty())
      return std::list<RT::Intersection>();

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

RT::AbstractTree const *  RT::createIntersection(RT::AbstractTree const * A, RT::AbstractTree const * B)
{
  RT::IntersectionNode *  tree = new RT::IntersectionNode();

  // Push CSG trees in newly created CSG union
  tree->push(A);
  tree->push(B);

  return tree;
}

std::string	RT::IntersectionNode::dump() const
{
  std::stringstream stream;

  stream << "intersection(t = " << transformation().dump() << "){";

  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    if (it != _children.begin())
      stream << ", ";
    stream << (*it)->dump();
  }

  stream << "}";

  return stream.str();
}