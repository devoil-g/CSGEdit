#ifndef _TANGLE_LEAF_HPP_
#define _TANGLE_LEAF_HPP_

#include <string>
#include <vector>

#include "AbstractLeaf.hpp"
#include "Ray.hpp"

namespace RT
{
  class TangleLeaf : public RT::AbstractLeaf
  {
  private:
    double const	_c;	// Constant used in tangle equation

    std::vector<double>	intersection(RT::Ray const &) const override;	// Render intersection distance according to ray
    Math::Vector<4>	normal(Math::Vector<4> const &) const override;	// Calculate normal from intersection point

  public:
    TangleLeaf(double);
    ~TangleLeaf();

    std::string		dump() const override;	// Dump CSG tree
  };
};

#endif
