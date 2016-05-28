#ifndef _BOX_CSG_LEAF_HPP_
#define _BOX_CSG_LEAF_HPP_

#include "AbstractCsgLeaf.hpp"

namespace RT
{
  class BoxCsgLeaf : public RT::AbstractCsgLeaf
  {
  private:
    double const	_x, _y, _z;	// Box dimension according to X, Y and Z axis
    bool const		_center;	// false= (0,0,0) at origin of the box, true= (0,0,0) at center of the box

    std::vector<double>	intersection(RT::Ray const &) const override;	// Render intersection distance according to ray
    Math::Vector<4>	normal(Math::Vector<4> const &) const override;	// Calculate normal from intersection point

  public:
    BoxCsgLeaf(double, double, double, bool);
    BoxCsgLeaf(double, bool);
    ~BoxCsgLeaf();

    size_t		build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const override;	// Build OpenCL data structure
  };
};

#endif
