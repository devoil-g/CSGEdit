#include "ExternLightTree.hpp"

RT::ExternLightTree::ExternLightTree(RT::AbstractLightTree const * const * tree)
  : _tree(tree)
{}

RT::ExternLightTree::~ExternLightTree()
{}

RT::Color	RT::ExternLightTree::preview(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int deph) const
{
  if (deph < RT::Config::Light::MaxDeph)
    return (*_tree)->preview(transformation, scene, ray, intersection, recursivite, deph + 1);
  else
    return RT::Color(0.f);
}

RT::Color	RT::ExternLightTree::render(Math::Matrix<4, 4> const & transformation, RT::Scene const * scene, RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite, unsigned int deph) const
{
  if (deph < RT::Config::Light::MaxDeph)
    return (*_tree)->render(transformation, scene, ray, intersection, recursivite, deph + 1);
  else
    return RT::Color(0.f);
}
