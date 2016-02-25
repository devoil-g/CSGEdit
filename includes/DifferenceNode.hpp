#ifndef _DIFFERENCE_NODE_HPP_
#define _DIFFERENCE_NODE_HPP_

#include <list>
#include <string>

#include "AbstractNode.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class DifferenceNode : public RT::AbstractNode
  {
  private:
    std::list<RT::Intersection>	renderChildren(RT::Ray const &) const override;	// Render sub-tree

  public:
    DifferenceNode();
    ~DifferenceNode();

    std::string			dump() const override;					// Dump CSG tree
  };
};

#endif
