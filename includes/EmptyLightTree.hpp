#ifndef _EMPTY_LIGHT_TREE_HPP_
#define _EMPTY_LIGHT_TREE_HPP_

#include "AbstractLightTree.hpp"

namespace RT
{
  class EmptyLightTree : public RT::AbstractLightTree
  {
  public:
    EmptyLightTree();
    ~EmptyLightTree();

    RT::Color	preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Return preview lighting for given intersection
    RT::Color	render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Return rendered lighting for given intersection
  };
};

#endif
