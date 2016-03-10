#include "TriangleLeaf.hpp"

RT::TriangleLeaf::TriangleLeaf(std::tuple<double, double, double> const & p0, std::tuple<double, double, double> const & p1, std::tuple<double, double, double> const & p2)
  : _p0(p0), _p1(p1), _p2(p2)
{
  // Calculate intersection using cross product
  _v0.x() = std::get<0>(_p1) - std::get<0>(_p0);
  _v0.y() = std::get<1>(_p1) - std::get<1>(_p0);
  _v0.z() = std::get<2>(_p1) - std::get<2>(_p0);

  _v1.x() = std::get<0>(_p2) - std::get<0>(_p0);
  _v1.y() = std::get<1>(_p2) - std::get<1>(_p0);
  _v1.z() = std::get<2>(_p2) - std::get<2>(_p0);

  _normal = RT::Ray::vectoriel(_v0, _v1);
}

RT::TriangleLeaf::~TriangleLeaf()
{}

std::vector<double>	RT::TriangleLeaf::intersection(RT::Ray const & ray) const
{
  Math::Vector<4>	v2;

  // Very clever method to get intersection distance
  v2.x() = ray.p().x() - std::get<0>(_p0);
  v2.y() = ray.p().y() - std::get<1>(_p0);
  v2.z() = ray.p().z() - std::get<2>(_p0);

  double		d = -(_normal.x() * ray.d().x() + _normal.y() * ray.d().y() + _normal.z() * ray.d().z());
  
  Math::Vector<4>	vec1 = RT::Ray::vectoriel(v2, _v1);
  double		a = -(vec1.x() * ray.d().x() + vec1.y() * ray.d().y() + vec1.z() * ray.d().z()) / d;

  if (a < 0.f || a > 1.f)
    return {};

  Math::Vector<4>	vec2 = RT::Ray::vectoriel(_v0, v2);
  double		b = -(vec2.x() * ray.d().x() + vec2.y() * ray.d().y() + vec2.z() * ray.d().z()) / d;

  // Push intersection if inside the triangle
  if (b > 0.f && a + b < 1.f)
    return { (_normal.x() * v2.x() + _normal.y() * v2.y() + _normal.z() * v2.z()) / d };
  else
    return {};
}

Math::Vector<4>	RT::TriangleLeaf::normal(Math::Vector<4> const &) const
{
  return _normal;
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