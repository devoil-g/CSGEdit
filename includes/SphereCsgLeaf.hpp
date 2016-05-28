#ifndef _SPHERE_CSG_LEAF_HPP_
#define _SPHERE_CSG_LEAF_HPP_

#include "AbstractCsgLeaf.hpp"

namespace RT
{
  class SphereCsgLeaf : public RT::AbstractCsgLeaf
  {
  private:
    double const	_r;	// Radius

    std::vector<double>	intersection(RT::Ray const &) const override;	// Render intersection distance according to ray
    Math::Vector<4>	normal(Math::Vector<4> const &) const override;	// Calculate normal from intersection point

  public:
    SphereCsgLeaf(double);
    ~SphereCsgLeaf();

    size_t		build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const override;	// Build OpenCL data structure
  };
};

#endif
