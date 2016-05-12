#include "DephLightNode.hpp"

RT::DephLightNode::DephLightNode(unsigned int deph)
  : _deph(deph)
{}

RT::DephLightNode::~DephLightNode()
{}

RT::Color	RT::DephLightNode::preview(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int deph) const
{
  if (deph < _deph)
    return RT::UnionLightNode::preview(transformation, scene, ray, intersection, recursivite, deph);
  else
    return RT::Color(0.f);
}

RT::Color	RT::DephLightNode::render(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int deph) const
{
  if (deph < _deph)
    return RT::UnionLightNode::render(transformation, scene, ray, intersection, recursivite, deph);
  else
    return RT::Color(0.f);
}
