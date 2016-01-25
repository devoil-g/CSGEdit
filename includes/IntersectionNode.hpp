#ifndef _INTERSECTION_NODE_HPP_
#define _INTERSECTION_NODE_HPP_

#include <list>

#include "AbstractNode.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"

namespace RT
{
  class IntersectionNode : public RT::AbstractNode
  {
  private:
    std::list<RT::Intersection>	renderTree(Math::Ray const &) const override;  // Render sub-tree

  public:
    IntersectionNode();
    IntersectionNode(Math::Matrix<4, 4> const &);
    ~IntersectionNode();

    std::string		dump() const override;
  };

  RT::AbstractTree const *  createIntersection(RT::AbstractTree const *, RT::AbstractTree const *);
};

#endif
