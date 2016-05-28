#ifndef _TRIANGLE_CSG_LEAF_HPP_
#define _TRIANGLE_CSG_LEAF_HPP_

#include <tuple>
#include <vector>

#include "AbstractCsgLeaf.hpp"
#include "Vector.hpp"

namespace RT
{
  class TriangleCsgLeaf : public RT::AbstractCsgLeaf
  {
  private:
    std::tuple<double, double, double> const		_p0, _p1, _p2;		// Description of the three points of the triangle
    Math::Vector<4>					_v0, _v1, _normal;	// Pre-computed value for acceleration

    std::vector<double>					intersection(RT::Ray const &) const override;	// Render intersection distance according to ray
    Math::Vector<4>					normal(Math::Vector<4> const &) const override;	// Calculate normal from intersection point

  public:
    TriangleCsgLeaf(std::tuple<double, double, double> const &, std::tuple<double, double, double> const &, std::tuple<double, double, double> const &);
    ~TriangleCsgLeaf();

    size_t						build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const override;	// Build OpenCL data structure

    std::vector<std::tuple<double, double, double>>	points() const;		// Return vector of 3 points coordinates
  };
};

#endif
