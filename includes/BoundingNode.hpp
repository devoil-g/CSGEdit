#ifndef _BOUNDING_NODE_HPP_
#define _BOUNDING_NODE_HPP_

#include <list>
#include <string>

#include "AbstractNode.hpp"
#include "AbstractTree.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class BoundingNode : public AbstractNode
  {
  private:
    std::list<RT::Intersection>	renderChildren(Math::Ray const &) const override;	// Render sub-tree

  public:
    BoundingNode();
    ~BoundingNode();

    std::string			dump() const override;					// Dump CSG tree
  };
};

#endif