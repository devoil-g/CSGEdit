#include "SphereLeaf.hpp"

RT::SphereLeaf::SphereLeaf(double r)
  : _r(r)
{}

RT::SphereLeaf::SphereLeaf(Math::Matrix<4, 4> const & transformation, double r)
  : AbstractLeaf(transformation), _r(r)
{}

RT::SphereLeaf::~SphereLeaf()
{}

std::vector<double>	RT::SphereLeaf::intersection(Math::Ray const & ray) const
{
  double		a, b, c;

  // Solve sphere equation X²+Y²+Z²=R²
  a = ray.dx() * ray.dx() + ray.dy() * ray.dy() + ray.dz() * ray.dz();
  b = 2.f * (ray.px() * ray.dx() + ray.py() * ray.dy() + ray.pz() * ray.dz());
  c = ray.px() * ray.px() + ray.py() * ray.py() + ray.pz() * ray.pz() - _r * _r;

  return Math::Utils::solve(a, b, c);
}

Math::Ray	RT::SphereLeaf::normal(Math::Ray const & ray) const
{
  Math::Ray	normal;

  // Derive sphere equation
  normal.dx() = 2 * ray.px();
  normal.dy() = 2 * ray.py();
  normal.dz() = 2 * ray.pz();

  return normal;
}
