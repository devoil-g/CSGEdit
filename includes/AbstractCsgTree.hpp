#ifndef _ABSTRACT_CSG_TREE_HPP_
#define _ABSTRACT_CSG_TREE_HPP_

#include <list>

#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class AbstractCsgTree
  {
  public:
    AbstractCsgTree();
    virtual ~AbstractCsgTree();

    virtual std::list<RT::Intersection>	render(RT::Ray const &, unsigned int = 0) const = 0;	// Render a list of intersection from the sub-tree according to ray
  };
};

#endif
