#ifndef _INTERSECTION_CSG_NODE_HPP_
#define _INTERSECTION_CSG_NODE_HPP_

#include "AbstractCsgNode.hpp"

namespace RT
{
  class IntersectionCsgNode : public RT::AbstractCsgNode
  {
  private:
    std::list<RT::Intersection>	renderChildren(RT::Ray const &, unsigned int) const override;	// Render sub-tree

  public:
    IntersectionCsgNode();
    ~IntersectionCsgNode();
  };
};

#endif
