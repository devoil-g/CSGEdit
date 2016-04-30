#ifndef _EXTERN_LIGHT_TREE_HPP_
#define _EXTERN_LIGHT_TREE_HPP_

#include "AbstractLightTree.hpp"

namespace RT
{
  class ExternLightTree : public RT::AbstractLightTree
  {
  private:
    RT::AbstractLightTree const * const &	_tree;	// Pointer to a mesh in MeshManager

  public:
    ExternLightTree(RT::AbstractLightTree const * const &);
    ~ExternLightTree();

    RT::Color	preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Return preview lighting for given intersection
    RT::Color	render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Return rendered lighting for given intersection
  };
};

#endif
