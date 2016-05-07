#include "UnionLightNode.hpp"

RT::UnionLightNode::UnionLightNode()
{}

RT::UnionLightNode::~UnionLightNode()
{}

RT::Color	RT::UnionLightNode::preview(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int deph) const
{
  RT::Color	clr = 0.f;

  // Iterate through children
  for (RT::AbstractLightTree const * it : _children)
    clr += it->preview(transformation, scene, ray, intersection, recursivite, deph);

  return clr;
}

RT::Color	RT::UnionLightNode::render(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int deph) const
{
  RT::Color	clr = 0.f;

  // Iterate through children
  for (RT::AbstractLightTree const * it : _children)
    clr += it->render(transformation, scene, ray, intersection, recursivite, deph);

  return clr;
}
