#ifndef _DIFFERENCE_CSG_NODE_HPP_
#define _DIFFERENCE_CSG_NODE_HPP_

#include "AbstractCsgNode.hpp"

namespace RT
{
  class DifferenceCsgNode : public RT::AbstractCsgNode
  {
  public:
    DifferenceCsgNode();
    ~DifferenceCsgNode();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render sub-tree
  };
};

#endif
