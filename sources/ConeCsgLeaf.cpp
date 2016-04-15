#include <sstream>

#include "ConeCsgLeaf.hpp"
#include "Math.hpp"

RT::ConeCsgLeaf::ConeCsgLeaf(double r, double h, bool center)
  : _r1(r), _r2(r), _h(h), _center(center)
{}

RT::ConeCsgLeaf::ConeCsgLeaf(double r1, double r2, double h, bool center)
  : _r1(r1), _r2(r2), _h(h), _center(center)
{}

RT::ConeCsgLeaf::~ConeCsgLeaf()
{}

std::vector<double>	RT::ConeCsgLeaf::intersection(RT::Ray const & ray) const
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

  // Stop if no intersection with infinite cone/cylinder
  if (tmp.size() == 0)
    return std::vector<double>();

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

Math::Vector<4>	RT::ConeCsgLeaf::normal(Math::Vector<4> const & pt) const
{
  Math::Vector<4>	p;

  // Apply center to ray
  p = pt;
  if (_center == true)
    p.z() += _h / 2.f;

  // If intersection with cylinder/cone
  if (p.z() > Math::Shift && p.z() < _h - Math::Shift)
  {
    double	x = _r1 + (_r2 - _r1) * p.z() / _h - _r2;
    
    return Math::Vector<4>(2.f * p.x(), 2.f * p.y(), x + _r2 > 0 ? -2.f * std::sqrt(p.x() * p.x() + p.y() * p.y()) * (-x) / (_h - p.z()) : +2.f * std::sqrt(p.x() * p.x() + p.y() * p.y()) * (-x) / (_h - p.z()), 0.f);
  }
  // If intersection with top/base disk
  else
    return Math::Vector<4>(0.f, 0.f, p.z() <= Math::Shift ? -1.f : +1.f, 0.f);
}
