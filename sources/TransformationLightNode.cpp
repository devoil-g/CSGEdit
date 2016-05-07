#include "TransformationLightNode.hpp"

RT::TransformationLightNode::TransformationLightNode(Math::Matrix<4, 4> const & transformation)
  : _transformation(transformation)
{}

RT::TransformationLightNode::~TransformationLightNode()
{}

RT::Color	RT::TransformationLightNode::preview(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int deph) const
{
  return RT::UnionLightNode::preview(transformation * _transformation, scene, ray, intersection, recursivite, deph);
}

RT::Color	RT::TransformationLightNode::render(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int deph) const
{
  return RT::UnionLightNode::render(transformation * _transformation, scene, ray, intersection, recursivite, deph);
}
