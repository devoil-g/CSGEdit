#ifndef _ABSTRACT_TREE_HPP_
#define _ABSTRACT_TREE_HPP_

#include <list>
#include <string>

#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class AbstractTree
  {
  public:
    AbstractTree();
    virtual ~AbstractTree();

    virtual std::list<RT::Intersection>	render(Math::Ray const &) const = 0;	// Render a list of intersection from the sub-tree according to ray
    virtual std::string			dump() const = 0;			// Dump CSG tree
  };
};

#endif
