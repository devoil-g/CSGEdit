#ifndef _CONE_LEAF_HPP_
#define _CONE_LEAF_HPP_

#include <vector>

#include "AbstractLeaf.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"

namespace RT
{
  class ConeLeaf : public RT::AbstractLeaf
  {
  private:
    double const	_r1, _r2, _h; // Radius of base, top, and height
    bool const		_center;      // false = (0,0,0) at center of base, true = (0,0,0) at mid-height

    std::vector<double>	intersection(Math::Ray const &) const override;	// Render intersection distance according to ray
    Math::Ray		normal(Math::Ray const &) const override;	// Calculate normal from intersection point

  public:
    ConeLeaf(double, double, bool);
    ConeLeaf(double, double, double, bool);
    ConeLeaf(Math::Matrix<4, 4> const &, double, double, bool);
    ConeLeaf(Math::Matrix<4, 4> const &, double, double, double, bool);
    ~ConeLeaf();

    std::string		dump() const override;
  };
};

#endif
