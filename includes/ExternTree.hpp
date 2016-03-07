#ifndef _EXTERN_TREE_HPP_
#define _EXTERN_TREE_HPP_

#include <list>

#include "AbstractTree.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class ExternTree : public RT::AbstractTree
  {
  private:
    RT::AbstractTree const * const &	_tree;	// Pointer to a mesh in MeshManager
    
  public:
    ExternTree(RT::AbstractTree const * const &);
    ~ExternTree();

    std::list<RT::Intersection>	render(RT::Ray const &) const override;	// Render a list of intersection from the sub-tree according to ray
    std::string			dump() const override;			// Dump CSG tree
  };
};

#endif
