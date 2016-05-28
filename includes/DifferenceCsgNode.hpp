#ifndef _DIFFERENCE_CSG_NODE_HPP_
#define _DIFFERENCE_CSG_NODE_HPP_

#include "AbstractCsgNode.hpp"

namespace RT
{
  class DifferenceCsgNode : public RT::AbstractCsgNode
  {
  public:
    DifferenceCsgNode();
    ~DifferenceCsgNode();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;															// Render sub-tree
    size_t			build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const override;	// Build OpenCL data structure
  };
};

#endif
