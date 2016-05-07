#ifndef _EXTERN_CSG_TREE_HPP_
#define _EXTERN_CSG_TREE_HPP_

#include "AbstractCsgTree.hpp"

namespace RT
{
  class ExternCsgTree : public RT::AbstractCsgTree
  {
  private:
    RT::AbstractCsgTree const * const &	_tree;	// Pointer to a mesh in MeshManager

  public:
    ExternCsgTree(RT::AbstractCsgTree const * const &);
    ~ExternCsgTree();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render a list of intersection from the sub-tree according to ray
  };
};

#endif
