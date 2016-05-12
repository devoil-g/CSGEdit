#ifndef _ABSTRACT_LIGHT_NODE_HPP_
#define _ABSTRACT_LIGHT_NODE_HPP_

#include <list>

#include "AbstractLightTree.hpp"

namespace RT
{
  class AbstractLightNode : public RT::AbstractLightTree
  {
  private:
    std::list<RT::AbstractLightTree *>	_children;	// List of children CSG tree

  public:
    AbstractLightNode();
    virtual ~AbstractLightNode();

    virtual void	push(RT::AbstractLightTree *);	// Add a CSG tree to sub-trees list
    virtual void	pop();				// Pop last CSG tree from sub-trees list

    // Getter of children list
    inline std::list<RT::AbstractLightTree *> &		children() { return _children; };
    inline std::list<RT::AbstractLightTree *> const &	children() const { return _children; };
  };
};

#endif
