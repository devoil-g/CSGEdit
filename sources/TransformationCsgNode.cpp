#include "TransformationCsgNode.hpp"

RT::TransformationCsgNode::TransformationCsgNode(Math::Matrix<4, 4> const & transformation)
  : _transformation(transformation)
{}

RT::TransformationCsgNode::~TransformationCsgNode()
{}

std::list<RT::Intersection>	RT::TransformationCsgNode::render(RT::Ray const & ray, unsigned int deph) const
{
  std::list<RT::Intersection>	result = RT::UnionCsgNode::render(_transformation.inverse() * ray, deph);

  // Inverse transformation
  for (RT::Intersection & it : result)
  {
    it.normal.p() = _transformation * it.normal.p();
    it.normal.d() = _transformation.inverse().transpose() * it.normal.d();
    it.normal = it.normal.normalize();
  }

  return result;
}
