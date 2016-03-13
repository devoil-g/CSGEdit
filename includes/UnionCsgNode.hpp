#ifndef _UNION_CSG_NODE_HPP_
#define _UNION_CSG_NODE_HPP_

#include "AbstractCsgNode.hpp"

namespace RT
{
  class UnionCsgNode : public RT::AbstractCsgNode
  {
  private:
    std::list<RT::Intersection>	renderChildren(RT::Ray const &, unsigned int) const override;	// Render sub-tree

  public:
    UnionCsgNode();
    ~UnionCsgNode();
  };
};

#endif
