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
    std::list<RT::Intersection>	renderTree(Math::Ray const &) const override;  // Render sub-tree

  public:
    DifferenceNode();
    DifferenceNode(Math::Matrix<4, 4> const &);
    ~DifferenceNode();

    std::string		dump() const override;
  };

  RT::AbstractTree const *   createDifference(RT::AbstractTree const *, RT::AbstractTree const *);
};

#endif
