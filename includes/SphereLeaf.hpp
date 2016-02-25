#ifndef _SPHERE_LEAF_HPP_
#define _SPHERE_LEAF_HPP_

#include <string>
#include <vector>

#include "AbstractLeaf.hpp"
#include "Ray.hpp"

namespace RT
{
  class SphereLeaf : public RT::AbstractLeaf
  {
  private:
    double const	_r;						// Radius

    std::vector<double>	intersection(RT::Ray const &) const override;	// Render intersection distance according to ray
    Math::Vector<4>	normal(Math::Vector<4> const &) const override;	// Calculate normal from intersection point

  public:
    SphereLeaf(double);
    ~SphereLeaf();

    std::string		dump() const override;				// Dump CSG tree
  };
};

#endif
