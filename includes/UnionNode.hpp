#ifndef _UNION_NODE_HPP_
#define _UNION_NODE_HPP_

#include <list>

#include "AbstractNode.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"

namespace RT
{
  class UnionNode : public RT::AbstractNode
  {
  private:
    std::list<RT::Intersection>	renderTree(Math::Ray const &) const override;  // Render sub-tree

  public:
    UnionNode();
    UnionNode(Math::Matrix<4, 4> const &);
    ~UnionNode();

    std::string		dump() const override;
  };

  RT::AbstractTree const *  createUnion(RT::AbstractTree const *, RT::AbstractTree const *);
};

#endif
