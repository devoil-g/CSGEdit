#ifndef _BOUNDING_CSG_NODE_HPP_
#define _BOUNDING_CSG_NODE_HPP_

#include "UnionCsgNode.hpp"

namespace RT
{
  class BoundingCsgNode : public RT::UnionCsgNode
  {
  private:
    RT::AbstractCsgTree *	_bound;

  public:
    BoundingCsgNode();
    ~BoundingCsgNode();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render sub-tree

    void			push(RT::AbstractCsgTree *) override;																	// Use the first CsgTree pushed as bound
    size_t			build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const override;	// Build OpenCL data structure
  };
};

#endif
