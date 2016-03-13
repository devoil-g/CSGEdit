#ifndef _DIFFERENCE_CSG_NODE_HPP_
#define _DIFFERENCE_CSG_NODE_HPP_

#include "AbstractCsgNode.hpp"

namespace RT
{
  class DifferenceCsgNode : public RT::AbstractCsgNode
  {
  private:
    std::list<RT::Intersection>	renderChildren(RT::Ray const &, unsigned int) const override;	// Render sub-tree

  public:
    DifferenceCsgNode();
    ~DifferenceCsgNode();
  };
};

#endif
