#ifndef _ABSTRACT_TREE_HPP_
#define _ABSTRACT_TREE_HPP_

#include <list>

#include "Intersection.hpp"
#include "Matrix.hpp"

namespace RT
{
  class AbstractTree
  {
  private:
    Math::Matrix<4, 4> const	_transformation;  // Transformation matrix applied to sub-tree

    virtual std::list<RT::Intersection>	renderTree(Math::Ray const &) const = 0;  // Call by render method to calculate intersection. You should override this method.

  public:
    AbstractTree();
    AbstractTree(Math::Matrix<4, 4> const &);
    virtual ~AbstractTree();

    std::list<RT::Intersection>	render(Math::Ray const &) const;  // Render a list of intersection from the sub-tree according to ray
  };
};

#endif
