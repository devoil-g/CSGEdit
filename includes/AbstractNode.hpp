#ifndef _ABSTRACT_NODE_HPP_
#define _ABSTRACT_NODE_HPP_

#include <list>

#include "AbstractTree.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"

namespace RT
{
  class AbstractNode : public RT::AbstractTree
  {
  private:
    virtual std::list<RT::Intersection>	renderTree(Math::Ray const &) const = 0;  // Render intersections list according to ray

  protected:
    std::list<RT::AbstractTree const *>	_children; // List of sub-trees

  public:
    AbstractNode();
    AbstractNode(Math::Matrix<4, 4> const &);
    virtual ~AbstractNode();

    void  push(RT::AbstractTree const *); // Add a CSG tree to sub-trees list
  };
};

#endif
