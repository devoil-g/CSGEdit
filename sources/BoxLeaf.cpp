#include <sstream>

#include "BoxLeaf.hpp"
#include "Math.hpp"

RT::BoxLeaf::BoxLeaf(double x, double y, double z, bool center)
  : _x(x), _y(y), _z(z), _center(center)
{}

RT::BoxLeaf::BoxLeaf(double l, bool center)
  : _x(l), _y(l), _z(l), _center(center)
{}

RT::BoxLeaf::~BoxLeaf()
{}

std::vector<double>	RT::BoxLeaf::intersection(RT::Ray const & ray) const
{
  std::vector<double>	result;
  RT::Ray		r = ray;

  // Apply center to ray
  if (_center == true)
  {
    r.p().x() += _x / 2.f;
    r.p().y() += _y / 2.f;
    r.p().z() += _z / 2.f;
  }

  // Intersection with X faces
  if (r.d().x() != 0)
  {
    // Calculate intersection distance
    double	x1 = (-r.p().x()) / r.d().x();
    double	x2 = (_x - r.p().x()) / r.d().x();

    // Push intersections if inside faces
    if (r.p().y() + x1 * r.d().y() >= 0 &&
      r.p().y() + x1 * r.d().y() < _y &&
      r.p().z() + x1 * r.d().z() >= 0 &&
      r.p().z() + x1 * r.d().z() < _z)
      result.push_back(x1);
    if (r.p().y() + x2 * r.d().y() >= 0 &&
      r.p().y() + x2 * r.d().y() < _y &&
      r.p().z() + x2 * r.d().z() >= 0 &&
      r.p().z() + x2 * r.d().z() < _z)
      result.push_back(x2);
  }

  // Intersection with Y faces
  if (r.d().y() != 0)
  {
    // Calculate intersection distance
    double	y1 = (-r.p().y()) / r.d().y();
    double	y2 = (_y - r.p().y()) / r.d().y();

    // Push intersections if inside faces
    if (r.p().x() + y1 * r.d().x() >= 0 &&
      r.p().x() + y1 * r.d().x() < _x &&
      r.p().z() + y1 * r.d().z() >= 0 &&
      r.p().z() + y1 * r.d().z() < _z)
      result.push_back(y1);
    if (r.p().x() + y2 * r.d().x() >= 0 &&
      r.p().x() + y2 * r.d().x() < _x &&
      r.p().z() + y2 * r.d().z() >= 0 &&
      r.p().z() + y2 * r.d().z() < _z)
      result.push_back(y2);
  }

  // Intersection with Z faces
  if (r.d().z() != 0)
  {
    // Calculate intersection distance
    double	z1 = (-r.p().z()) / r.d().z();
    double	z2 = (_z - r.p().z()) / r.d().z();

    // Push intersections if inside faces
    if (r.p().x() + z1 * r.d().x() >= 0 &&
      r.p().x() + z1 * r.d().x() < _x &&
      r.p().y() + z1 * r.d().y() >= 0 &&
      r.p().y() + z1 * r.d().y() < _y)
      result.push_back(z1);
    if (r.p().x() + z2 * r.d().x() >= 0 &&
      r.p().x() + z2 * r.d().x() < _x &&
      r.p().y() + z2 * r.d().y() >= 0 &&
      r.p().y() + z2 * r.d().y() < _y)
      result.push_back(z2);
  }

  return result;
}

Math::Vector<4>	RT::BoxLeaf::normal(Math::Vector<4> const & pt) const
{
  RT::Ray	n;

  // Apply center to ray
  n.p() = pt;
  if (_center == true)
  {
    n.p().x() += _x / 2.f;
    n.p().y() += _y / 2.f;
    n.p().z() += _z / 2.f;
  }

  // Find which face is intersected (X, Y or Z)
  if (n.p().x() < Math::Shift)
    n.d().x() = -1;
  else if (n.p().x() > _x - Math::Shift)
    n.d().x() = +1;
  else if (n.p().y() < Math::Shift)
    n.d().y() = -1;
  else if (n.p().y() > _y - Math::Shift)
    n.d().y() = +1;
  else if (n.p().z() < Math::Shift)
    n.d().z() = -1;
  else
    n.d().z() = +1;

  return n.d();
}

std::string	RT::BoxLeaf::dump() const
{
  std::stringstream stream;

  stream << "box(" << _x << ", " << _y << ", " << _z << ", " << (_center ? "true" : "false") << ");";

  return stream.str();
}