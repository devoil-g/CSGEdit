#ifndef _UNION_CSG_NODE_HPP_
#define _UNION_CSG_NODE_HPP_

#include "AbstractCsgNode.hpp"

namespace RT
{
  class UnionCsgNode : public RT::AbstractCsgNode
  {
  public:
    UnionCsgNode();
    virtual ~UnionCsgNode();

    virtual std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;															// Render sub-tree
    virtual size_t			build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const override;	// Build OpenCL data structure
  };
};

#endif
