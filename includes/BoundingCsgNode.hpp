#ifndef _BOUNDING_CSG_NODE_HPP_
#define _BOUNDING_CSG_NODE_HPP_

#include "UnionCsgNode.hpp"

namespace RT
{
  class BoundingCsgNode : public RT::UnionCsgNode
  {
  private:
    RT::AbstractCsgTree *	_bound;

    std::list<RT::Intersection>	renderChildren(RT::Ray const &, unsigned int) const override;	// Render sub-tree

  public:
    BoundingCsgNode();
    ~BoundingCsgNode();

    void	push(RT::AbstractCsgTree *) override;	// Use the first CsgTree pushed as bound
  };
};

#endif
