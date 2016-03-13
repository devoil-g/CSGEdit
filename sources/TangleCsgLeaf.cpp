#include <sstream>

#include "Math.hpp"
#include "TangleCsgLeaf.hpp"

RT::TangleCsgLeaf::TangleCsgLeaf(double c)
  : _c(c)
{}

RT::TangleCsgLeaf::~TangleCsgLeaf()
{}

std::vector<double>	RT::TangleCsgLeaf::intersection(RT::Ray const & ray) const
{
  // Resolve tangle equation X^4 - 5X² + Y^4 - 5Y² + Z^4 - 5Z² + C = 0
  return Math::Utils::solve(
    std::pow(ray.d().x(), 4.0) + std::pow(ray.d().y(), 4.0) + std::pow(ray.d().z(), 4.f),
    4.f * (std::pow(ray.d().x(), 3.f) * ray.p().x() + std::pow(ray.d().y(), 3.f) * ray.p().y() + std::pow(ray.d().z(), 3.f) * ray.p().z()),
    6.f * (ray.d().x() * ray.d().x() * ray.p().x() * ray.p().x() + ray.d().y() * ray.d().y() * ray.p().y() * ray.p().y() + ray.d().z() * ray.d().z() * ray.p().z() * ray.p().z()) - 5.f * (ray.d().x() * ray.d().x() + ray.d().y() * ray.d().y() + ray.d().z() * ray.d().z()),
    4.f * (std::pow(ray.p().x(), 3.f) * ray.d().x() + std::pow(ray.p().y(), 3.f) * ray.d().y() + std::pow(ray.p().z(), 3.f) * ray.d().z()) - 10.f * (ray.d().x() * ray.p().x() + ray.d().y() * ray.p().y() + ray.d().z() * ray.p().z()),
    std::pow(ray.p().x(), 4.f) + std::pow(ray.p().y(), 4.f) + std::pow(ray.p().z(), 4.f) - 5.f * (ray.p().x() * ray.p().x() + ray.p().y() * ray.p().y() + ray.p().z() * ray.p().z()) + _c
    );
}

Math::Vector<4>	RT::TangleCsgLeaf::normal(Math::Vector<4> const & pt) const
{
  return pt * pt * pt * 4.f - pt * 10.f;
}
