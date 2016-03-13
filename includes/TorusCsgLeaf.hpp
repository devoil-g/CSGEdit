#ifndef _TORUS_CSG_LEAF_HPP_
#define _TORUS_CSG_LEAF_HPP_

#include "AbstractCsgLeaf.hpp"

namespace RT
{
  class TorusCsgLeaf : public RT::AbstractCsgLeaf
  {
  private:
    double const	_r, _h;	// Big and small radius

    std::vector<double>	intersection(RT::Ray const &) const override;	// Render intersection distance according to ray
    Math::Vector<4>	normal(Math::Vector<4> const &) const override;	// Calculate normal from intersection point

  public:
    TorusCsgLeaf(double, double);
    ~TorusCsgLeaf();
  };
};

#endif
