#include <unordered_map>

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
  for (RT::AbstractTree const * it : _children)
    intersect.merge(it->render(r));

  std::unordered_map<RT::AbstractTree const *, bool>	inside;
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

std::string	RT::TransformationNode::dump() const
{
  std::stringstream stream;

  stream << "transformation(" << _transformation.dump() << ");";

  for (RT::AbstractTree const * it : _children)
    stream << it->dump();

  stream << "end();";

  return stream.str();
}

Math::Matrix<4, 4>  RT::TransformationNode::transformation() const
{
  return _transformation;
}