#include "TriangleLeaf.hpp"

RT::TriangleLeaf::TriangleLeaf(std::tuple<double, double, double> const & p0, std::tuple<double, double, double> const & p1, std::tuple<double, double, double> const & p2)
  : _p0(p0), _p1(p1), _p2(p2)
{
  // Calculate intersection using cross product
  _v0.d().x() = std::get<0>(_p1) - std::get<0>(_p0);
  _v0.d().y() = std::get<1>(_p1) - std::get<1>(_p0);
  _v0.d().z() = std::get<2>(_p1) - std::get<2>(_p0);

  _v1.d().x() = std::get<0>(_p2) - std::get<0>(_p0);
  _v1.d().y() = std::get<1>(_p2) - std::get<1>(_p0);
  _v1.d().z() = std::get<2>(_p2) - std::get<2>(_p0);

  _normal = RT::Ray::vectoriel(_v0, _v1);
}

RT::TriangleLeaf::~TriangleLeaf()
{}

std::vector<double>	RT::TriangleLeaf::intersection(RT::Ray const & ray) const
{
  RT::Ray	v2;

  // Very clever method to get intersection distance
  v2.d().x() = ray.p().x() - std::get<0>(_p0);
  v2.d().y() = ray.p().y() - std::get<1>(_p0);
  v2.d().z() = ray.p().z() - std::get<2>(_p0);

  double	d = -(_normal.d().x() * ray.d().x() + _normal.d().y() * ray.d().y() + _normal.d().z() * ray.d().z());
  
  RT::Ray	vec1 = RT::Ray::vectoriel(v2, _v1);
  double	a = -(vec1.d().x() * ray.d().x() + vec1.d().y() * ray.d().y() + vec1.d().z() * ray.d().z()) / d;

  if (a < 0.f || a > 1.f)
    return std::vector<double>();

  RT::Ray	vec2 = RT::Ray::vectoriel(_v0, v2);
  double	b = -(vec2.d().x() * ray.d().x() + vec2.d().y() * ray.d().y() + vec2.d().z() * ray.d().z()) / d;

  // Push intersection if inside the triangle
  if (b > 0.f && a + b < 1.f)
    return std::vector<double>({ (_normal.d().x() * v2.d().x() + _normal.d().y() * v2.d().y() + _normal.d().z() * v2.d().z()) / d });
  else
    return std::vector<double>();
}

Math::Vector<4>	RT::TriangleLeaf::normal(Math::Vector<4> const &) const
{
  return _normal.d();
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