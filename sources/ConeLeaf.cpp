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

std::vector<double>	RT::ConeLeaf::intersection(RT::Ray const & ray) const
{
  std::vector<double>	result, tmp;
  RT::Ray		r = ray;
  
  // Apply centering to ray
  if (_center == true)
    r.p().z() += _h / 2.f;

  // If cylinder
  if (_r1 == _r2)
    tmp = Math::Utils::solve(
      r.d().x() * r.d().x() + r.d().y() * r.d().y(),
      2.f * (r.p().x() * r.d().x() + r.p().y() * r.d().y()),
      r.p().x() * r.p().x() + r.p().y() * r.p().y() - _r1 * _r1
      );
  // If cone
  else
    tmp = Math::Utils::solve(
      r.d().x() * r.d().x() + r.d().y() * r.d().y() - r.d().z() * r.d().z() * (_r2 - _r1) * (_r2 - _r1) / (_h * _h),
      2 * (+r.p().x() * r.d().x() + r.p().y() * r.d().y() - r.p().z() * r.d().z() * (_r2 - _r1) * (_r2 - _r1) / (_h * _h) - r.d().z() * _r1 * (_r2 - _r1) / _h),
      r.p().x() * r.p().x() + r.p().y() * r.p().y() - r.p().z() * r.p().z() * (_r2 - _r1) * (_r2 - _r1) / (_h * _h) - 2 * r.p().z() * _r1 *  (_r2 - _r1) / _h - _r1 * _r1
      );

  // Add intersection to result if corresponding to height
  for (unsigned int d = 0; d < tmp.size(); d++)
    if (r.p().z() + tmp[d] * r.d().z() >= 0 && r.p().z() + tmp[d] * r.d().z() <= _h)
      result.push_back(tmp[d]);

  if (r.d().z() != 0)
  {
    // Calculate top and bottom disk intersections
    double	x1 = (-r.p().z()) / r.d().z();
    double	x2 = (_h - r.p().z()) / r.d().z();

    // Add intersections if inside cylinder/cone
    if (+(r.p().x() + x1 * r.d().x()) * (r.p().x() + x1 * r.d().x())
      + (r.p().y() + x1 * r.d().y()) * (r.p().y() + x1 * r.d().y()) < _r1 * _r1)
      result.push_back(x1);
    if (+(r.p().x() + x2 * r.d().x()) * (r.p().x() + x2 * r.d().x())
      + (r.p().y() + x2 * r.d().y()) * (r.p().y() + x2 * r.d().y()) < _r2 * _r2)
      result.push_back(x2);
  }

  return result;
}

Math::Vector<4>	RT::ConeLeaf::normal(Math::Vector<4> const & pt) const
{
  RT::Ray	n;

  // Apply center to ray
  n.p() = pt;
  if (_center == true)
    n.p().z() += _h / 2.f;

  // If intersection with cylinder/cone
  if (n.p().z() > Math::Shift && n.p().z() < _h - Math::Shift)
  {
    double	x, y;

    x = _r1 + (_r2 - _r1) * n.p().z() / _h - _r2;
    y = _h - n.p().z();

    n.d().x() = 2 * n.p().x();
    n.d().y() = 2 * n.p().y();
    n.d().z() = std::sqrt(n.d().x() * n.d().x() + n.d().y() * n.d().y()) * (-x) / y;

    if (x + _r2 > 0)
      n.d().z() *= -1;
  }
  // If intersection with top/base disk
  else
  {
    n.d().x() = 0;
    n.d().y() = 0;

    if (n.p().z() <= Math::Shift)
      n.d().z() = -1;
    else
      n.d().z() = +1;
  }

  return n.d();
}

std::string	RT::ConeLeaf::dump() const
{
  std::stringstream stream;

  if (_r1 == _r2)
    stream << "cylinder(" << _r1 << ", " << _h << ", " << (_center ? "true" : "false") << ");";
  else
    stream << "cone(" << _r1 << ", " << _r2 << ", " << _h << ", " << (_center ? "true" : "false") << ");";

  return stream.str();
}