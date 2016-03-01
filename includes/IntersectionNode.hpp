#ifndef _INTERSECTION_NODE_HPP_
#define _INTERSECTION_NODE_HPP_

#include <list>
#include <string>

#include "AbstractNode.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class IntersectionNode : public RT::AbstractNode
  {
  private:
    std::list<RT::Intersection>	renderChildren(RT::Ray const &) const override;	// Render sub-tree

  public:
    IntersectionNode();
    ~IntersectionNode();

    std::string			dump() const override;	// Dump CSG tree
  };
};

#endif
