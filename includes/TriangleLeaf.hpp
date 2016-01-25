#ifndef _TRIANGLE_LEAF_HPP_
#define _TRIANGLE_LEAF_HPP_

#include <tuple>
#include <vector>

#include "AbstractLeaf.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"

namespace RT
{
  class TriangleLeaf : public RT::AbstractLeaf
  {
  private:
    std::tuple<double, double, double> const	_p0, _p1, _p2;	// Description of the three points of the triangle

    std::vector<double>	intersection(Math::Ray const &) const override;	// Render intersection distance according to ray
    Math::Ray		normal(Math::Ray const &) const override;	// Calculate normal from intersection point

  public:
    TriangleLeaf(std::tuple<double, double, double> const &, std::tuple<double, double, double> const &, std::tuple<double, double, double> const &);
    TriangleLeaf(Math::Matrix<4, 4> const &, std::tuple<double, double, double> const &, std::tuple<double, double, double> const &, std::tuple<double, double, double> const &);
    ~TriangleLeaf();

    std::string		dump() const override;
  };
};

#endif
