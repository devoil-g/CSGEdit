#ifndef _ABSTRACT_NODE_HPP_
#define _ABSTRACT_NODE_HPP_

#include <list>

#include "AbstractTree.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class AbstractNode : public RT::AbstractTree
  {
  private:
    virtual std::list<RT::Intersection>	renderChildren(Math::Ray const &) const = 0;	// Render children

  protected:
    std::list<RT::AbstractTree const *>	_children;					// List of children CSG tree

  public:
    AbstractNode();
    virtual ~AbstractNode();

    std::list<RT::Intersection>	render(Math::Ray const &) const;			// Render a list of intersection from the sub-tree according to ray
    void  push(RT::AbstractTree const *);						// Add a CSG tree to sub-trees list
  };
};

#endif
