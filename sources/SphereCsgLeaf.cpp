#include "SphereCsgLeaf.hpp"

RT::SphereCsgLeaf::SphereCsgLeaf(double r)
  : _r(r)
{}

RT::SphereCsgLeaf::~SphereCsgLeaf()
{}

std::vector<double>	RT::SphereCsgLeaf::intersection(RT::Ray const & ray) const
{
  // Solve sphere equation X²+Y²+Z²=R²
  return Math::solve(
    std::pow(ray.d().x(), 2) + std::pow(ray.d().y(), 2) + std::pow(ray.d().z(), 2),
    2.f * (ray.p().x() * ray.d().x() + ray.p().y() * ray.d().y() + ray.p().z() * ray.d().z()),
    std::pow(ray.p().x(), 2) + std::pow(ray.p().y(), 2) + std::pow(ray.p().z(), 2) - std::pow(_r, 2)
  );
}

Math::Vector<4>	RT::SphereCsgLeaf::normal(Math::Vector<4> const & pt) const
{
  return pt;
}
