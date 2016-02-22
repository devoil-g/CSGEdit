#include <map>

#include "TransformationNode.hpp"

RT::TransformationNode::TransformationNode(Math::Matrix<4, 4> const & transformation)
  : _transformation(transformation)
{}

RT::TransformationNode::~TransformationNode()
{}

std::list<RT::Intersection>	RT::TransformationNode::renderChildren(Math::Ray const & ray) const
{
  std::list<std::list<RT::Intersection> > intersect_list;
  Math::Ray const &			  r = _transformation.inverse() * ray;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(r);

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

  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
    stream << (*it)->dump();

  stream << "end();";

  return stream.str();
}

Math::Matrix<4, 4>  RT::TransformationNode::transformation() const
{
  return _transformation;
}