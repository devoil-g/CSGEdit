#include "EmptyLightTree.hpp"

RT::EmptyLightTree::EmptyLightTree()
{}

RT::EmptyLightTree::~EmptyLightTree()
{}

RT::Color	RT::EmptyLightTree::preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const
{
  return RT::Color(0.f);
}

RT::Color	RT::EmptyLightTree::render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const
{
  return RT::Color(0.f);
}
