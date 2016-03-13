#ifndef _SPHERE_CSG_LEAF_HPP_
#define _SPHERE_CSG_LEAF_HPP_

#include <string>
#include <vector>

#include "AbstractCsgLeaf.hpp"
#include "Ray.hpp"

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
  };
};

#endif
