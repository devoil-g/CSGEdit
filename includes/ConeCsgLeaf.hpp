#ifndef _CONE_CSG_LEAF_HPP_
#define _CONE_CSG_LEAF_HPP_

#include "AbstractCsgLeaf.hpp"

namespace RT
{
  class ConeCsgLeaf : public RT::AbstractCsgLeaf
  {
  private:
    double const	_r1, _r2, _h;	// Radius of base, top, and height
    bool const		_center;	// false = (0,0,0) at center of base, true = (0,0,0) at mid-height

    std::vector<double>	intersection(RT::Ray const &) const override;	// Render intersection distance according to ray
    Math::Vector<4>	normal(Math::Vector<4> const &) const override;	// Calculate normal from intersection point

  public:
    ConeCsgLeaf(double, double, bool);
    ConeCsgLeaf(double, double, double, bool);
    ~ConeCsgLeaf();

    size_t		build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const override;	// Build OpenCL data structure
  };
};

#endif
