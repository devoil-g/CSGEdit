#include "ConeCsgLeaf.hpp"

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
      std::pow(r.d().x(), 2) + std::pow(r.d().y(), 2),
      2.f * (r.p().x() * r.d().x() + r.p().y() * r.d().y()),
      std::pow(r.p().x(), 2) + std::pow(r.p().y(), 2) - std::pow(_r1, 2)
      );
  // If cone
  else
    tmp = Math::Utils::solve(
      std::pow(r.d().x(), 2) + std::pow(r.d().y(), 2) - std::pow(r.d().z() * (_r2 - _r1), 2) / std::pow(_h, 2),
      2.f * (r.p().x() * r.d().x() + r.p().y() * r.d().y() - r.p().z() * r.d().z() * std::pow((_r2 - _r1) / _h, 2) - r.d().z() * _r1 * (_r2 - _r1) / _h),
      std::pow(r.p().x(), 2) + std::pow(r.p().y(), 2) - std::pow(r.p().z() * (_r2 - _r1) / _h, 2) - 2.f * r.p().z() * _r1 * (_r2 - _r1) / _h - std::pow(_r1, 2)
      );

  // Stop if no intersection with infinite cone/cylinder
  if (tmp.size() == 0)
    return std::vector<double>();

  // Add intersection to result if corresponding to height
  for (double it : tmp)
    if (r.p().z() + it * r.d().z() >= 0.f && r.p().z() + it * r.d().z() <= _h)
      result.push_back(it);
  
  if (r.d().z() != 0.f)
  {
    // Calculate top and bottom disk intersections
    double	x1 = (-r.p().z()) / r.d().z();
    double	x2 = (_h - r.p().z()) / r.d().z();

    // Add intersections if inside cylinder/cone
    if (std::pow(r.p().x() + x1 * r.d().x(), 2) + std::pow(r.p().y() + x1 * r.d().y(), 2) < std::pow(_r1, 2))
      result.push_back(x1);
    if (std::pow(r.p().x() + x2 * r.d().x(), 2) + std::pow(r.p().y() + x2 * r.d().y(), 2) < std::pow(_r2, 2))
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
