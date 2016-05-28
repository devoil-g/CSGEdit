#ifndef _DEPH_CSG_NODE_HPP_
#define _DEPH_CSG_NODE_HPP_

#include "UnionCsgNode.hpp"

namespace RT
{
  class DephCsgNode : public RT::UnionCsgNode
  {
  private:
    unsigned int const		_deph;	// Maximum deph before interrupting rendering

  public:
    DephCsgNode(unsigned int);
    ~DephCsgNode();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;															// Render sub-tree
    size_t			build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const override;	// Build OpenCL data structure
  };
};

#endif
