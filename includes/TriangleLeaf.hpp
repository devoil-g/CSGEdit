#ifndef _TRIANGLE_LEAF_HPP_
#define _TRIANGLE_LEAF_HPP_

#include <string>
#include <tuple>
#include <vector>

#include "AbstractLeaf.hpp"
#include "Ray.hpp"
#include "Vector.hpp"

namespace RT
{
  class TriangleLeaf : public RT::AbstractLeaf
  {
  private:
    std::tuple<double, double, double> const		_p0, _p1, _p2;		// Description of the three points of the triangle
    Math::Vector<4>					_v0, _v1, _normal;	// Pre-computed value for acceleration

    std::vector<double>					intersection(RT::Ray const &) const override;	// Render intersection distance according to ray
    Math::Vector<4>					normal(Math::Vector<4> const &) const override;	// Calculate normal from intersection point

  public:
    TriangleLeaf(std::tuple<double, double, double> const &, std::tuple<double, double, double> const &, std::tuple<double, double, double> const &);
    ~TriangleLeaf();

    std::string						dump() const override;	// Dump CSG tree
    std::vector<std::tuple<double, double, double>>	points() const;		// Return vector of 3 points coordinates
  };
};

#endif
