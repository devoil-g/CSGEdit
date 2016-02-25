#include "TriangleLeaf.hpp"

RT::TriangleLeaf::TriangleLeaf(std::tuple<double, double, double> const & p0, std::tuple<double, double, double> const & p1, std::tuple<double, double, double> const & p2)
  : _p0(p0), _p1(p1), _p2(p2)
{}

RT::TriangleLeaf::~TriangleLeaf()
{}

std::vector<double>	RT::TriangleLeaf::intersection(RT::Ray const & ray) const
{
  std::vector<double>	result;
  RT::Ray		v, v0, v1, v2;
  double		a, b, c, d;

  // Very clever method to get intersection distance
  v0.d().x() = std::get<0>(_p1) - std::get<0>(_p0);
  v0.d().y() = std::get<1>(_p1) - std::get<1>(_p0);
  v0.d().z() = std::get<2>(_p1) - std::get<2>(_p0);

  v1.d().x() = std::get<0>(_p2) - std::get<0>(_p0);
  v1.d().y() = std::get<1>(_p2) - std::get<1>(_p0);
  v1.d().z() = std::get<2>(_p2) - std::get<2>(_p0);

  v2.d().x() = ray.p().x() - std::get<0>(_p0);
  v2.d().y() = ray.p().y() - std::get<1>(_p0);
  v2.d().z() = ray.p().z() - std::get<2>(_p0);

  v = RT::Ray::vectoriel(v0, v1);
  d = -(v.d().x() * ray.d().x() + v.d().y() * ray.d().y() + v.d().z() * ray.d().z());
  c = (v.d().x() * v2.d().x() + v.d().y() * v2.d().y() + v.d().z() * v2.d().z()) / d;

  v = RT::Ray::vectoriel(v2, v1);
  a = -(v.d().x() * ray.d().x() + v.d().y() * ray.d().y() + v.d().z() * ray.d().z()) / d;

  v = RT::Ray::vectoriel(v0, v2);
  b = -(v.d().x() * ray.d().x() + v.d().y() * ray.d().y() + v.d().z() * ray.d().z()) / d;

  // Push intersection if inside the triangle
  if (a > 0 && b > 0 && a + b < 1)
    result.push_back(c);
  
  return result;
}

Math::Vector<4>	RT::TriangleLeaf::normal(Math::Vector<4> const &) const
{
  RT::Ray	v0, v1;

  // Calculate intersection using cross product
  v0.d().x() = std::get<0>(_p1) - std::get<0>(_p0);
  v0.d().y() = std::get<1>(_p1) - std::get<1>(_p0);
  v0.d().z() = std::get<2>(_p1) - std::get<2>(_p0);

  v1.d().x() = std::get<0>(_p2) - std::get<0>(_p0);
  v1.d().y() = std::get<1>(_p2) - std::get<1>(_p0);
  v1.d().z() = std::get<2>(_p2) - std::get<2>(_p0);

  return RT::Ray::vectoriel(v0, v1).d();
}

std::string	RT::TriangleLeaf::dump() const
{
  std::stringstream stream;

  stream << "triangle([" << std::get<0>(_p0) << ", " << std::get<1>(_p0) << ", " << std::get<2>(_p0) << "], [" << std::get<0>(_p1) << ", " << std::get<1>(_p1) << ", " << std::get<2>(_p1) << "], [" << std::get<0>(_p2) << ", " << std::get<1>(_p2) << ", " << std::get<2>(_p2) << "]);";

  return stream.str();
}

std::vector<std::tuple<double, double, double>>	RT::TriangleLeaf::points() const
{
  std::vector<std::tuple<double, double, double>> result;

  result.push_back(std::tuple<double, double, double>(std::get<0>(_p0), std::get<1>(_p0), std::get<2>(_p0)));
  result.push_back(std::tuple<double, double, double>(std::get<0>(_p1), std::get<1>(_p1), std::get<2>(_p1)));
  result.push_back(std::tuple<double, double, double>(std::get<0>(_p2), std::get<1>(_p2), std::get<2>(_p2)));

  return result;
}