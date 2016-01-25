#ifndef _ABSTRACT_TREE_HPP_
#define _ABSTRACT_TREE_HPP_

#include <list>
#include <string>
#include <ostream>

#include "Intersection.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"

namespace RT
{
  class AbstractTree
  {
  private:
    Math::Matrix<4, 4> const	_transformation;  // Transformation matrix applied to sub-tree

    virtual std::list<RT::Intersection>	renderTree(Math::Ray const &) const = 0;  // Call by render method to calculate intersection. You should override this method.

  protected:
    Math::Matrix<4, 4> const &	transformation() const;

  public:
    AbstractTree();
    AbstractTree(Math::Matrix<4, 4> const &);
    virtual ~AbstractTree();

    std::list<RT::Intersection>	render(Math::Ray const &) const;  // Render a list of intersection from the sub-tree according to ray
    virtual std::string		dump() const = 0;
  };
};

#endif
