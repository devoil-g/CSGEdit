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

size_t				RT::TransformationCsgNode::build(std::vector<RT::OpenCL::Node> & nodes, std::vector<RT::OpenCL::Primitive> & primitives, Math::Matrix<4, 4> const & transformation, RT::Material const & material, unsigned int deph) const
{
  return RT::UnionCsgNode::build(nodes, primitives, transformation * _transformation, material, deph);
}