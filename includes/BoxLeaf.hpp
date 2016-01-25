#ifndef _BOX_LEAF_HPP_
#define _BOX_LEAF_HPP_

#include <vector>

#include "AbstractLeaf.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"

namespace RT
{
  class BoxLeaf : public RT::AbstractLeaf
  {
  private:
    double const	_x, _y, _z; // Box dimension according to X, Y and Z axis
    bool const		_center;    // false= (0,0,0) at origin of the box, true= (0,0,0) at center of the box

    std::vector<double>	intersection(Math::Ray const &) const override;	// Render intersection distance according to ray
    Math::Ray		normal(Math::Ray const &) const override;	// Calculate normal from intersection point

  public:
    BoxLeaf(double, double, double, bool);
    BoxLeaf(double, bool);
    BoxLeaf(Math::Matrix<4, 4> const &, double, double, double, bool);
    BoxLeaf(Math::Matrix<4, 4> const &, double, bool);
    ~BoxLeaf();

    std::string		dump() const override;
  };
};

#endif
