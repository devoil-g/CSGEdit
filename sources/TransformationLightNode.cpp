#include "AbstractLightTree.hpp"
#include "Color.hpp"
#include "Matrix.hpp"
#include "Scene.hpp"
#include "TransformationLightNode.hpp"

RT::TransformationLightNode::TransformationLightNode(Math::Matrix<4, 4> const & transformation)
  : _transformation(transformation)
{}

RT::TransformationLightNode::~TransformationLightNode()
{}

RT::Color		RT::TransformationLightNode::preview(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int deph) const
{
  Math::Matrix<4, 4>	matrix = transformation * _transformation;
  RT::Color		clr = 0.f;

  for (RT::AbstractLightTree const * it : _children)
    clr += it->preview(matrix, scene, ray, intersection, recursivite, deph);

  return clr;
}

RT::Color		RT::TransformationLightNode::render(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int deph) const
{
  Math::Matrix<4, 4>	matrix = transformation * _transformation;
  RT::Color		clr = 0.f;

  for (RT::AbstractLightTree const * it : _children)
    clr += it->render(matrix, scene, ray, intersection, recursivite, deph);

  return clr;
}
