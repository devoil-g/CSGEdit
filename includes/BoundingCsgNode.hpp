#ifndef _BOUNDING_CSG_NODE_HPP_
#define _BOUNDING_CSG_NODE_HPP_

#include "UnionCsgNode.hpp"

namespace RT
{
  class BoundingCsgNode : public RT::UnionCsgNode
  {
  private:
    RT::AbstractCsgTree *	_bound;

  public:
    BoundingCsgNode();
    ~BoundingCsgNode();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render sub-tree

    void			push(RT::AbstractCsgTree *) override;	// Use the first CsgTree pushed as bound
  };
};

#endif
