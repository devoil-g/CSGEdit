#ifndef _SPHERE_LEAF_HPP_
#define _SPHERE_LEAF_HPP_

#include <vector>

#include "AbstractLeaf.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"

namespace RT
{
  class SphereLeaf : public RT::AbstractLeaf
  {
  private:
    double const	_r; // Radius

    std::vector<double>	intersection(Math::Ray const &) const override;	// Render intersection distance according to ray
    Math::Ray		normal(Math::Ray const &) const override;	// Calculate normal from intersection point

  public:
    SphereLeaf(double);
    SphereLeaf(Math::Matrix<4, 4> const &, double);
    ~SphereLeaf();
  };
};

#endif
