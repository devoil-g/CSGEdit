#include <unordered_map>

#include "TransformationCsgNode.hpp"

RT::TransformationCsgNode::TransformationCsgNode(Math::Matrix<4, 4> const & transformation)
  : _transformation(transformation)
{}

RT::TransformationCsgNode::~TransformationCsgNode()
{}

std::list<RT::Intersection>	RT::TransformationCsgNode::renderChildren(RT::Ray const & ray, unsigned int deph) const
{
  std::list<RT::Intersection>	intersect;
  RT::Ray const &		r = _transformation.inverse() * ray;

  // Iterate through sub-tree to get intersections
  for (RT::AbstractCsgTree const * it : _children)
    intersect.merge(it->render(r, deph));

  std::unordered_map<RT::AbstractCsgTree const *, bool>	inside;
  std::list<RT::Intersection>				result;
  unsigned int						state = 0;

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

  for (RT::Intersection & it : result)
  {
    // Inverse transformation
    it.normal.p() = _transformation * it.normal.p();
    it.normal.d() = _transformation.inverse().transpose() * it.normal.d();
    it.normal = it.normal.normalize();
  }

  return result;
}

Math::Matrix<4, 4>  RT::TransformationCsgNode::transformation() const
{
  return _transformation;
}
