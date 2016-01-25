#ifndef _TORUS_LEAF_HPP_
#define _TORUS_LEAF_HPP_

#include <vector>

#include "AbstractLeaf.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"

namespace RT
{
  class TorusLeaf : public RT::AbstractLeaf
  {
  private:
    double const	_r, _h;	// Big and small radius

    std::vector<double>	intersection(Math::Ray const &) const override;	// Render intersection distance according to ray
    Math::Ray		normal(Math::Ray const &) const override;	// Calculate normal from intersection point

  public:
    TorusLeaf(double, double);
    TorusLeaf(Math::Matrix<4, 4> const &, double, double);
    ~TorusLeaf();

    std::string		dump() const override;
  };
};

#endif
