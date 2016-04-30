#include <exception>

#include "Math.hpp"
#include "MobiusCsgLeaf.hpp"

RT::MobiusCsgLeaf::MobiusCsgLeaf(double r1, double r2, double t)
  : _r1(r1), _r2(r2), _t(t)
{
  if (_r2 > _r1 / 2.f)
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
  if (_t > _r2)
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
}

RT::MobiusCsgLeaf::~MobiusCsgLeaf()
{}

std::vector<double>	RT::MobiusCsgLeaf::intersection(RT::Ray const & ray) const
{
  std::vector<double>	result;

  for (double it : Math::Utils::solve(
    ray.d().x() * ray.d().x() * ray.d().y() + ray.d().y() * ray.d().y() * ray.d().y() - 2.f * ray.d().x() * ray.d().x() * ray.d().z() - 2.f * ray.d().y() * ray.d().y() * ray.d().z() + ray.d().y() * ray.d().z() * ray.d().z(),
    2.f * ray.d().x() * ray.d().y() * ray.p().x() - 4.f * ray.d().x() * ray.d().z() * ray.p().x() + ray.d().x() * ray.d().x() * ray.p().y() + 3.f * ray.d().y() * ray.d().y() * ray.p().y() - 4.f * ray.d().y() * ray.d().z() * ray.p().y() + ray.d().z() * ray.d().z() * ray.p().y() - 2.f * ray.d().x() * ray.d().x() * ray.p().z() - 2.f * ray.d().y() * ray.d().y() * ray.p().z() + 2.f * ray.d().y() * ray.d().z() * ray.p().z() - 2.f * ray.d().x() * ray.d().z() * _r1,
    ray.d().y() * ray.p().x() * ray.p().x() - 2.f * ray.d().z() * ray.p().x() * ray.p().x() + 2.f * ray.d().x() * ray.p().x() * ray.p().y() + 3.f * ray.d().y() * ray.p().y() * ray.p().y() - 2.f * ray.d().z() * ray.p().y() * ray.p().y() - 4.f * ray.d().x() * ray.p().x() * ray.p().z() - 4.f * ray.d().y() * ray.p().y() * ray.p().z() + 2.f * ray.d().z() * ray.p().y() * ray.p().z() + ray.d().y() * ray.p().z() * ray.p().z() - 2.f * ray.d().z() * ray.p().x() * _r1 - 2.f * ray.d().x() * ray.p().z() * _r1 - ray.d().y() * _r1 * _r1,
    ray.p().x() * ray.p().x() * ray.p().y() + ray.p().y() * ray.p().y() * ray.p().y() - 2.f * ray.p().x() * ray.p().x() * ray.p().z() - 2.f * ray.p().y() * ray.p().y() * ray.p().z() + ray.p().y() * ray.p().z() * ray.p().z() - 2.f * ray.p().x() * ray.p().z() * _r1 - ray.p().y() * _r1 * _r1
  ))
  {
    Math::Vector<4>	pt = ray.p() + ray.d() * it;
    double		angle = std::atan2(pt.y(), pt.x());
    double		s;

    if (std::abs(std::sin(angle / 2.f)) > Math::Shift)
      s = pt.z() / std::sin(angle / 2.f);
    else if (std::abs(std::cos(angle)) > Math::Shift)
      s = (pt.x() / std::cos(angle) - _r1) / std::cos(angle / 2.f);
    else
      s = (pt.y() / std::sin(angle) - _r1) / std::cos(angle / 2.f);

    if (std::abs(std::pow(pt.x() - (_r1 + s * std::cos(angle / 2.f)) * std::cos(angle), 2) + std::pow(pt.y() - (_r1 + s * std::cos(angle / 2.f)) * std::sin(angle), 2) + std::pow(pt.z() - s * std::sin(angle / 2), 2)) < Math::Shift && std::abs(s) < _r2 + Math::Shift)
    {
      result.push_back(it);
      result.push_back(it);
    }
  }

  return result;
}

Math::Vector<4>		RT::MobiusCsgLeaf::normal(Math::Vector<4> const & pt) const
{
  return Math::Vector<4>(2.f * pt.x() * pt.y() - 2.f * _r1 * pt.z() - 4.f * pt.x() * pt.z(), -_r1 * _r1 + pt.x() * pt.x() + 3.f * pt.y() * pt.y() - 4.f * pt.y() * pt.z() + pt.z() * pt.z(), -2.f * _r1 * pt.x() - 2.f * pt.x() * pt.x() - 2.f * pt.y() * pt.y() + 2.f * pt.y() * pt.z(), 0.f);
}
