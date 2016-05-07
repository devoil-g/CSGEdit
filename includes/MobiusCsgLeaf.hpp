#ifndef _MOBIUS_CSG_LEAF_HPP_
#define _MOBIUS_CSG_LEAF_HPP_

#include "AbstractCsgLeaf.hpp"

namespace RT
{
  class MobiusCsgLeaf : public RT::AbstractCsgLeaf
  {
  private:
    double const	_r1, _r2;	// Radius
    double const	_t;		// Thickness

    std::vector<double>	intersection(RT::Ray const &) const override;	// Render intersection distance according to ray
    Math::Vector<4>	normal(Math::Vector<4> const &) const override;	// Calculate normal from intersection point

  public:
    MobiusCsgLeaf(double, double, double);
    ~MobiusCsgLeaf();
  };
};

#endif
