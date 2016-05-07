#ifndef _ABSTRACT_LIGHT_NODE_HPP_
#define _ABSTRACT_LIGHT_NODE_HPP_

#include <list>

#include "AbstractLightTree.hpp"

namespace RT
{
  class AbstractLightNode : public RT::AbstractLightTree
  {
  protected:
    std::list<RT::AbstractLightTree *>	_children;	// List of children CSG tree

  public:
    AbstractLightNode();
    virtual ~AbstractLightNode();

    virtual void	push(RT::AbstractLightTree *);	// Add a CSG tree to sub-trees list
    virtual void	pop();				// Pop last CSG tree from sub-trees list
    virtual bool	empty() const;			// Return true if no children in list
  };
};

#endif
