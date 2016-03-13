#ifndef _BOUNDING_CSG_NODE_HPP_
#define _BOUNDING_CSG_NODE_HPP_

#include "AbstractCsgNode.hpp"

namespace RT
{
  class BoundingCsgNode : public RT::AbstractCsgNode
  {
  private:
    std::list<RT::Intersection>	renderChildren(RT::Ray const &, unsigned int) const override;	// Render sub-tree

  public:
    BoundingCsgNode();
    ~BoundingCsgNode();
  };
};

#endif
