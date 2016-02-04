#ifndef _DIFFERENCE_NODE_HPP_
#define _DIFFERENCE_NODE_HPP_

#include <list>

#include "AbstractNode.hpp"
#include "AbstractTree.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"

namespace RT
{
  class DifferenceNode : public RT::AbstractNode
  {
  private:
    std::list<RT::Intersection>	renderChildren(Math::Ray const &) const override;  // Render sub-tree

  public:
    DifferenceNode();
    ~DifferenceNode();

    std::string			dump() const override;
  };
};

#endif
