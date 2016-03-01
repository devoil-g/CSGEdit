#ifndef _UNION_NODE_HPP_
#define _UNION_NODE_HPP_

#include <list>

#include "AbstractNode.hpp"
#include "AbstractTree.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class UnionNode : public RT::AbstractNode
  {
  private:
    std::list<RT::Intersection>	renderChildren(RT::Ray const &) const override;	// Render sub-tree

  public:
    UnionNode();
    ~UnionNode();

    std::string			dump() const override;	// Dump CSG tree
  };
};

#endif
