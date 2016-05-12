#ifndef _INTERSECTION_CSG_NODE_HPP_
#define _INTERSECTION_CSG_NODE_HPP_

#include "AbstractCsgNode.hpp"

namespace RT
{
  class IntersectionCsgNode : public RT::AbstractCsgNode
  {
  public:
    IntersectionCsgNode();
    ~IntersectionCsgNode();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render sub-tree
  };
};

#endif
