#ifndef _DEPH_CSG_NODE_HPP_
#define _DEPH_CSG_NODE_HPP_

#include "UnionCsgNode.hpp"

namespace RT
{
  class DephCsgNode : public RT::UnionCsgNode
  {
  private:
    unsigned int const	_deph;	// Maximum deph before interrupting rendering

    std::list<RT::Intersection>	renderChildren(RT::Ray const &, unsigned int) const override;	// Render sub-tree

  public:
    DephCsgNode(unsigned int);
    ~DephCsgNode();
  };
};

#endif
