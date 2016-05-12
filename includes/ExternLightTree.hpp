#ifndef _EXTERN_LIGHT_TREE_HPP_
#define _EXTERN_LIGHT_TREE_HPP_

#include "AbstractLightTree.hpp"

namespace RT
{
  namespace Config
  {
    namespace Light
    {
      unsigned int const	MaxDeph(8);	// Maximum of sub-file light tree rendered
    };
  };

  class ExternLightTree : public RT::AbstractLightTree
  {
  private:
    RT::AbstractLightTree const * const &	_tree;	// Pointer to a light tree

  public:
    ExternLightTree(RT::AbstractLightTree const * const &);
    ~ExternLightTree();

    RT::Color	preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Return preview lighting for given intersection
    RT::Color	render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Return rendered lighting for given intersection
  };
};

#endif
