#include "TriangleLeaf.hpp"

RT::TriangleLeaf::TriangleLeaf(std::tuple<double, double, double> const & p0, std::tuple<double, double, double> const & p1, std::tuple<double, double, double> const & p2)
  : _p0(p0), _p1(p1), _p2(p2)
{}

RT::TriangleLeaf::~TriangleLeaf()
{}

std::vector<double>	RT::TriangleLeaf::intersection(Math::Ray const & ray) const
{
  std::vector<double>	result;
  Math::Ray		v, v0, v1, v2;
  double		a, b, c, d, l;

  // Very clever method to get intersection distance
  l = std::sqrt(ray.dx() * ray.dx() + ray.dy() * ray.dy() + ray.dz() * ray.dz());

  v0.dx() = std::get<0>(_p1) - std::get<0>(_p0);
  v0.dy() = std::get<1>(_p1) - std::get<1>(_p0);
  v0.dz() = std::get<2>(_p1) - std::get<2>(_p0);

  v1.dx() = std::get<0>(_p2) - std::get<0>(_p0);
  v1.dy() = std::get<1>(_p2) - std::get<1>(_p0);
  v1.dz() = std::get<2>(_p2) - std::get<2>(_p0);

  v2.dx() = ray.px() - std::get<0>(_p0);
  v2.dy() = ray.py() - std::get<1>(_p0);
  v2.dz() = ray.pz() - std::get<2>(_p0);

  v = Math::Ray::vectoriel(v0, v1);
  d = -(v.dx() * ray.dx() + v.dy() * ray.dy() + v.dz() * ray.dz());
  c = (v.dx() * v2.dx() + v.dy() * v2.dy() + v.dz() * v2.dz()) / d;

  v = Math::Ray::vectoriel(v2, v1);
  a = -(v.dx() * ray.dx() + v.dy() * ray.dy() + v.dz() * ray.dz()) / d;

  v = Math::Ray::vectoriel(v0, v2);
  b = -(v.dx() * ray.dx() + v.dy() * ray.dy() + v.dz() * ray.dz()) / d;

  // Push intersection if inside the triangle
  if (a > 0 && b > 0 && a + b < 1)
    result.push_back(c);
  
  return result;
}

Math::Ray	RT::TriangleLeaf::normal(Math::Ray const &) const
{
  Math::Ray	normal, v0, v1;

  // Calculate intersection using cross product
  v0.dx() = std::get<0>(_p1) - std::get<0>(_p0);
  v0.dy() = std::get<1>(_p1) - std::get<1>(_p0);
  v0.dz() = std::get<2>(_p1) - std::get<2>(_p0);

  v1.dx() = std::get<0>(_p2) - std::get<0>(_p0);
  v1.dy() = std::get<1>(_p2) - std::get<1>(_p0);
  v1.dz() = std::get<2>(_p2) - std::get<2>(_p0);

  normal = Math::Ray::vectoriel(v0, v1);

  return normal;
}

std::string	RT::TriangleLeaf::dump() const
{
  std::stringstream stream;

  stream << "triangle(p0 = [" << std::get<0>(_p0) << ", " << std::get<1>(_p0) << ", " << std::get<2>(_p0) << "], p1 = [" << std::get<0>(_p1) << ", " << std::get<1>(_p1) << ", " << std::get<2>(_p1) << "], p2 = [" << std::get<0>(_p2) << ", " << std::get<1>(_p2) << ", " << std::get<2>(_p2) << "])";

  return stream.str();
}