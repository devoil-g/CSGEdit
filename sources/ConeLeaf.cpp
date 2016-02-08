#include <sstream>

#include "ConeLeaf.hpp"
#include "Math.hpp"

RT::ConeLeaf::ConeLeaf(double r, double h, bool center)
  : _r1(r), _r2(r), _h(h), _center(center)
{}

RT::ConeLeaf::ConeLeaf(double r1, double r2, double h, bool center)
  : _r1(r1), _r2(r2), _h(h), _center(center)
{}

RT::ConeLeaf::~ConeLeaf()
{}

std::vector<double>	RT::ConeLeaf::intersection(Math::Ray const & ray) const
{
  std::vector<double>	result, tmp;
  Math::Ray		r;
  double		a, b, c;

  // Apply centering to ray
  r = ray;
  if (_center == true)
    r.pz() += _h / 2.f;

  // If cylinder
  if (_r1 == _r2)
  {
    a = r.dx() * r.dx() + r.dy() * r.dy();
    b = 2.f * (r.px() * r.dx() + r.py() * r.dy());
    c = r.px() * r.px() + r.py() * r.py() - _r1 * _r1;
  }
  // If cone
  else
  {
    a = r.dx() * r.dx() + r.dy() * r.dy() - r.dz() * r.dz() * (_r2 - _r1) * (_r2 - _r1) / (_h * _h);
    b = 2 * (+r.px() * r.dx() + r.py() * r.dy() - r.pz() * r.dz() * (_r2 - _r1) * (_r2 - _r1) / (_h * _h) - r.dz() * _r1 * (_r2 - _r1) / _h);
    c = r.px() * r.px() + r.py() * r.py() - r.pz() * r.pz() * (_r2 - _r1) * (_r2 - _r1) / (_h * _h) - 2 * r.pz() * _r1 *  (_r2 - _r1) / _h - _r1 * _r1;
  }

  tmp = Math::Utils::solve(a, b, c);

  // Add intersection to result if corresponding to height
  for (unsigned int d = 0; d < tmp.size(); d++)
    if (r.pz() + tmp[d] * r.dz() >= 0 && r.pz() + tmp[d] * r.dz() <= _h)
      result.push_back(tmp[d]);

  if (r.dz() != 0)
  {
    // Calculate top and bottom disk intersections
    double	x1, x2;

    x1 = (-r.pz()) / r.dz();
    x2 = (_h - r.pz()) / r.dz();

    // Add intersections if inside cylinder/cone
    if (+(r.px() + x1 * r.dx()) * (r.px() + x1 * r.dx())
      + (r.py() + x1 * r.dy()) * (r.py() + x1 * r.dy()) < _r1 * _r1)
      result.push_back(x1);
    if (+(r.px() + x2 * r.dx()) * (r.px() + x2 * r.dx())
      + (r.py() + x2 * r.dy()) * (r.py() + x2 * r.dy()) < _r2 * _r2)
      result.push_back(x2);
  }

  return result;
}

Math::Ray	RT::ConeLeaf::normal(Math::Ray const & ray) const
{
  Math::Ray	normal;

  // Apply center to ray
  normal.p() = ray.p();
  if (_center == true)
    normal.pz() += _h / 2.f;

  // If intersection with cylinder/cone
  if (normal.pz() > Math::Shift && normal.pz() < _h - Math::Shift)
  {
    double	x, y;

    x = _r1 + (_r2 - _r1) * normal.pz() / _h - _r2;
    y = _h - normal.pz();

    normal.dx() = 2 * normal.px();
    normal.dy() = 2 * normal.py();
    normal.dz() = sqrt(normal.dx() * normal.dx() + normal.dy() * normal.dy()) * (-x) / y;

    if (x + _r2 > 0)
      normal.dz() *= -1;
  }
  // If intersection with top/base disk
  else
  {
    normal.dx() = 0;
    normal.dy() = 0;

    if (normal.pz() <= Math::Shift)
      normal.dz() = -1;
    else
      normal.dz() = +1;
  }

  return normal;
}

std::string	RT::ConeLeaf::dump() const
{
  std::stringstream stream;

  stream << "cone(r1 = " << _r1 << ", r2 = " << _r2 << ", h = " << _h << ", center = " << (_center ? "true" : "false") << ")";

  return stream.str();
}