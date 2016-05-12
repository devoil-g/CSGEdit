#ifndef _EXTERN_CSG_TREE_HPP_
#define _EXTERN_CSG_TREE_HPP_

#include "AbstractCsgTree.hpp"

namespace RT
{
  namespace Config
  {
    namespace Csg
    {
      unsigned int const	MaxDeph(8);	// Maximum of sub-file CSG tree rendered
    };
  };

  class ExternCsgTree : public RT::AbstractCsgTree
  {
  private:
    RT::AbstractCsgTree const * const &	_tree;	// Pointer to a csg tree

  public:
    ExternCsgTree(RT::AbstractCsgTree const * const &);
    ~ExternCsgTree();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render a list of intersection from the sub-tree according to ray
  };
};

#endif
