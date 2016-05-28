#ifndef _EMPTY_CSG_TREE_HPP_
#define _EMPTY_CSG_TREE_HPP_

#include "AbstractCsgTree.hpp"

namespace RT
{
  class EmptyCsgTree : public RT::AbstractCsgTree
  {
  public:
    EmptyCsgTree();
    ~EmptyCsgTree();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;															// Render a list of intersection from the sub-tree according to ray
    size_t			build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const override;	// Build OpenCL data structure
  };
};

#endif
