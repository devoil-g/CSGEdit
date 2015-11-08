#include "TangleLeaf.hpp"

RT::TangleLeaf::TangleLeaf(double c)
  : _c(c)
{}

RT::TangleLeaf::TangleLeaf(Math::Matrix<4, 4> const & transformation, double c)
  : AbstractLeaf(transformation), _c(c)
{}

RT::TangleLeaf::~TangleLeaf()
{}

std::vector<double>	RT::TangleLeaf::intersection(Math::Ray const & ray) const
{
  double		a, b, c, d, e;

  // Resolve tangle equation X^4 - 5X² + Y^4 - 5Y² + Z^4 - 5Z² + C = 0
  a = pow(ray.dx(), 4.0) + pow(ray.dy(), 4.0) + pow(ray.dz(), 4.f);
  b = 4.f * (pow(ray.dx(), 3.f) * ray.px() + pow(ray.dy(), 3.f) * ray.py() + pow(ray.dz(), 3.f) * ray.pz());
  c = 6.f * (ray.dx() * ray.dx() * ray.px() * ray.px() + ray.dy() * ray.dy() * ray.py() * ray.py() + ray.dz() * ray.dz() * ray.pz() * ray.pz()) - 5.f * (ray.dx() * ray.dx() + ray.dy() * ray.dy() + ray.dz() * ray.dz());
  d = 4.f * (pow(ray.px(), 3.f) * ray.dx() + pow(ray.py(), 3.f) * ray.dy() + pow(ray.pz(), 3.f) * ray.dz()) - 10.f * (ray.dx() * ray.px() + ray.dy() * ray.py() + ray.dz() * ray.pz());
  e = pow(ray.px(), 4.f) + pow(ray.py(), 4.f) + pow(ray.pz(), 4.f) - 5.f * (ray.px() * ray.px() + ray.py() * ray.py() + ray.pz() * ray.pz()) + _c;

  return Math::Utils::solve(a, b, c, d, e);
}

Math::Ray	RT::TangleLeaf::normal(Math::Ray const & ray) const
{
  Math::Ray	normal;

  // Derive tangle's equation
  normal.dx() = 4.f * pow(ray.px(), 3.f) - 10.f * ray.px();
  normal.dy() = 4.f * pow(ray.py(), 3.f) - 10.f * ray.py();
  normal.dz() = 4.f * pow(ray.pz(), 3.f) - 10.f * ray.pz();

  return normal;
}
