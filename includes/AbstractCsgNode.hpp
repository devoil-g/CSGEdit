#ifndef _ABSTRACT_CSG_NODE_HPP_
#define _ABSTRACT_CSG_NODE_HPP_

#include <list>

#include "AbstractCsgTree.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class AbstractCsgNode : public RT::AbstractCsgTree
  {
  private:
    virtual std::list<RT::Intersection>	renderChildren(RT::Ray const &, unsigned int) const = 0;	// Render children

  protected:
    std::list<RT::AbstractCsgTree *>	_children;	// List of children CSG tree

  public:
    AbstractCsgNode();
    virtual ~AbstractCsgNode();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render a list of intersection from the sub-tree according to ray

    virtual void	push(RT::AbstractCsgTree *);	// Add a CSG tree to sub-trees list
    virtual void	pop();				// Pop last CSG tree from sub-trees list

    // Getter of children list
    std::list<RT::AbstractCsgTree *> &		children() { return _children; };
    std::list<RT::AbstractCsgTree *> const &	children() const { return _children; };
  };
};

#endif
