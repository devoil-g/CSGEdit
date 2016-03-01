#ifndef _TORUS_LEAF_HPP_
#define _TORUS_LEAF_HPP_

#include <string>
#include <vector>

#include "AbstractLeaf.hpp"
#include "Ray.hpp"

namespace RT
{
  class TorusLeaf : public RT::AbstractLeaf
  {
  private:
    double const	_r, _h;	// Big and small radius

    std::vector<double>	intersection(RT::Ray const &) const override;	// Render intersection distance according to ray
    Math::Vector<4>	normal(Math::Vector<4> const &) const override;	// Calculate normal from intersection point

  public:
    TorusLeaf(double, double);
    ~TorusLeaf();

    std::string		dump() const override;	// Dump CSG tree
  };
};

#endif
