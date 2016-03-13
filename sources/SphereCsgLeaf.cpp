#include <sstream>

#include "Math.hpp"
#include "SphereCsgLeaf.hpp"

RT::SphereCsgLeaf::SphereCsgLeaf(double r)
  : _r(r)
{}

RT::SphereCsgLeaf::~SphereCsgLeaf()
{}

std::vector<double>	RT::SphereCsgLeaf::intersection(RT::Ray const & ray) const
{
  // Solve sphere equation X²+Y²+Z²=R²
  return Math::Utils::solve(
    ray.d().x() * ray.d().x() + ray.d().y() * ray.d().y() + ray.d().z() * ray.d().z(),
    2.f * (ray.p().x() * ray.d().x() + ray.p().y() * ray.d().y() + ray.p().z() * ray.d().z()),
    ray.p().x() * ray.p().x() + ray.p().y() * ray.p().y() + ray.p().z() * ray.p().z() - _r * _r
    );
}

Math::Vector<4>	RT::SphereCsgLeaf::normal(Math::Vector<4> const & pt) const
{
  return pt;
}
