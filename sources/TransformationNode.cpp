#include <map>

#include "TransformationNode.hpp"

RT::TransformationNode::TransformationNode(Math::Matrix<4, 4> const & transformation)
  : _transformation(transformation)
{}

RT::TransformationNode::~TransformationNode()
{}

std::list<RT::Intersection>	RT::TransformationNode::renderChildren(RT::Ray const & ray) const
{
  std::list<RT::Intersection>	intersect;
  RT::Ray const &		r = _transformation.inverse() * ray;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(r);

    // Atribute intersections to children
    for (std::list<RT::Intersection>::iterator it_node = node.begin(); it_node != node.end(); it_node++)
      it_node->node = *it;

    intersect.merge(node);
  }

  std::map<RT::AbstractTree const *, bool>  inside;
  std::list<RT::Intersection>		    result;
  unsigned int				    state = 0;

  // Iterate through intersections
  for (std::list<RT::Intersection>::iterator it = intersect.begin(); it != intersect.end(); it++)
  {
    // If currently outside, push intersection
    if (state == 0)
      result.push_back(*it);

    // Increment deepness if getting inside an object, decrement if getting outside
    if (inside[it->node])
      state--;
    else
      state++;
    inside[it->node] = !(inside[it->node]);

    // If currently outside, push intersection
    if (state == 0)
      result.push_back(*it);
  }

  for (std::list<RT::Intersection>::iterator it = result.begin(); it != result.end(); it++)
  {
    // Inverse transformation
    (*it).normal.p() = _transformation * (*it).normal.p();
    (*it).normal.d() = _transformation.inverse().transpose() * (*it).normal.d();
    (*it).normal = (*it).normal.normalize();
  }

  return result;
}

std::string	RT::TransformationNode::dump() const
{
  std::stringstream stream;

  stream << "transformation(" << _transformation.dump() << ");";

  for (std::list<RT::AbstractTree *>::const_iterator it = _children.begin(); it != _children.end(); it++)
    stream << (*it)->dump();

  stream << "end();";

  return stream.str();
}

Math::Matrix<4, 4>  RT::TransformationNode::transformation() const
{
  return _transformation;
}