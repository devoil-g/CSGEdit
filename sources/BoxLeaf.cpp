#include <sstream>

#include "BoxLeaf.hpp"

RT::BoxLeaf::BoxLeaf(double x, double y, double z, bool center)
  : _x(x), _y(y), _z(z), _center(center)
{}

RT::BoxLeaf::BoxLeaf(double l, bool center)
  : _x(l), _y(l), _z(l), _center(center)
{}

RT::BoxLeaf::BoxLeaf(Math::Matrix<4, 4> const & transformation, double x, double y, double z, bool center)
  : AbstractLeaf(transformation), _x(x), _y(y), _z(z), _center(center)
{}

RT::BoxLeaf::BoxLeaf(Math::Matrix<4, 4> const & transformation, double l, bool center)
  : AbstractLeaf(transformation), _x(l), _y(l), _z(l), _center(center)
{}

RT::BoxLeaf::~BoxLeaf()
{}

std::vector<double>	RT::BoxLeaf::intersection(Math::Ray const & ray) const
{
  std::vector<double>	result;
  Math::Ray		r;

  // Apply center to ray
  r = ray;
  if (_center == true)
  {
    r.px() += _x / 2.f;
    r.py() += _y / 2.f;
    r.pz() += _z / 2.f;
  }

  // Intersection with X faces
  if (r.dx() != 0)
  {
    double	x1, x2;

    // Calculate intersection distance
    x1 = (-r.px()) / r.dx();
    x2 = (_x - r.px()) / r.dx();

    // Push intersections if inside faces
    if (r.py() + x1 * r.dy() >= 0 &&
      r.py() + x1 * r.dy() < _y &&
      r.pz() + x1 * r.dz() >= 0 &&
      r.pz() + x1 * r.dz() < _z)
      result.push_back(x1);
    if (r.py() + x2 * r.dy() >= 0 &&
      r.py() + x2 * r.dy() < _y &&
      r.pz() + x2 * r.dz() >= 0 &&
      r.pz() + x2 * r.dz() < _z)
      result.push_back(x2);
  }

  // Intersection with Y faces
  if (r.dy() != 0)
  {
    double	y1, y2;

    // Calculate intersection distance
    y1 = (-r.py()) / r.dy();
    y2 = (_y - r.py()) / r.dy();

    // Push intersections if inside faces
    if (r.px() + y1 * r.dx() >= 0 &&
      r.px() + y1 * r.dx() < _x &&
      r.pz() + y1 * r.dz() >= 0 &&
      r.pz() + y1 * r.dz() < _z)
      result.push_back(y1);
    if (r.px() + y2 * r.dx() >= 0 &&
      r.px() + y2 * r.dx() < _x &&
      r.pz() + y2 * r.dz() >= 0 &&
      r.pz() + y2 * r.dz() < _z)
      result.push_back(y2);
  }

  // Intersection with Z faces
  if (r.dz() != 0)
  {
    double	z1, z2;

    // Calculate intersection distance
    z1 = (-r.pz()) / r.dz();
    z2 = (_z - r.pz()) / r.dz();

    // Push intersections if inside faces
    if (r.px() + z1 * r.dx() >= 0 &&
      r.px() + z1 * r.dx() < _x &&
      r.py() + z1 * r.dy() >= 0 &&
      r.py() + z1 * r.dy() < _y)
      result.push_back(z1);
    if (r.px() + z2 * r.dx() >= 0 &&
      r.px() + z2 * r.dx() < _x &&
      r.py() + z2 * r.dy() >= 0 &&
      r.py() + z2 * r.dy() < _y)
      result.push_back(z2);
  }

  return result;
}

Math::Ray	RT::BoxLeaf::normal(Math::Ray const & ray) const
{
  Math::Ray	normal;

  // Apply center to ray
  normal.p() = ray.p();
  if (_center == true)
  {
    normal.px() += _x / 2.f;
    normal.py() += _y / 2.f;
    normal.pz() += _z / 2.f;
  }

  normal.dx() = 0;
  normal.dy() = 0;
  normal.dz() = 0;

  // Find which face is intersected (X, Y or Z)
  if (normal.px() < Math::Shift)
    normal.dx() = -1;
  else if (normal.px() > _x - Math::Shift)
    normal.dx() = +1;
  else if (normal.py() < Math::Shift)
    normal.dy() = -1;
  else if (normal.py() > _y - Math::Shift)
    normal.dy() = +1;
  else if (normal.pz() < Math::Shift)
    normal.dz() = -1;
  else
    normal.dz() = +1;

  return normal;
}

std::string	RT::BoxLeaf::dump() const
{
  std::stringstream stream;

  stream << "box(t = " << transformation().dump() << ", x = " << _x << ", y = " << _y << ", z = " << _z << ", center = " << (_center ? "true" : "false") << ")";

  return stream.str();
}