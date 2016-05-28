#include "TriangleCsgLeaf.hpp"

RT::TriangleCsgLeaf::TriangleCsgLeaf(std::tuple<double, double, double> const & p0, std::tuple<double, double, double> const & p1, std::tuple<double, double, double> const & p2)
  : _p0(p0), _p1(p1), _p2(p2)
{
  // Calculate intersection using cross product
  _v0.x() = std::get<0>(_p1) - std::get<0>(_p0);
  _v0.y() = std::get<1>(_p1) - std::get<1>(_p0);
  _v0.z() = std::get<2>(_p1) - std::get<2>(_p0);

  _v1.x() = std::get<0>(_p2) - std::get<0>(_p0);
  _v1.y() = std::get<1>(_p2) - std::get<1>(_p0);
  _v1.z() = std::get<2>(_p2) - std::get<2>(_p0);

  // Pre-compute normal
  _normal = RT::Ray::cross(_v0, _v1);
}

RT::TriangleCsgLeaf::~TriangleCsgLeaf()
{}

std::vector<double>	RT::TriangleCsgLeaf::intersection(RT::Ray const & ray) const
{
  Math::Vector<4>	v2;

  // Very clever method to get intersection distance
  v2.x() = ray.p().x() - std::get<0>(_p0);
  v2.y() = ray.p().y() - std::get<1>(_p0);
  v2.z() = ray.p().z() - std::get<2>(_p0);

  double		d = -(_normal.x() * ray.d().x() + _normal.y() * ray.d().y() + _normal.z() * ray.d().z());

  Math::Vector<4>	vec1 = RT::Ray::cross(v2, _v1);
  double		a = -(vec1.x() * ray.d().x() + vec1.y() * ray.d().y() + vec1.z() * ray.d().z()) / d;

  if (a < 0.f || a > 1.f)
    return std::vector<double>();

  Math::Vector<4>	vec2 = RT::Ray::cross(_v0, v2);
  double		b = -(vec2.x() * ray.d().x() + vec2.y() * ray.d().y() + vec2.z() * ray.d().z()) / d;

  // Push intersection if inside the triangle
  if (b > 0.f && a + b < 1.f)
    return{ (_normal.x() * v2.x() + _normal.y() * v2.y() + _normal.z() * v2.z()) / d };
  else
    return std::vector<double>();
}

Math::Vector<4>	RT::TriangleCsgLeaf::normal(Math::Vector<4> const &) const
{
  // Return pre-computed normal
  return _normal;
}

std::vector<std::tuple<double, double, double>>	RT::TriangleCsgLeaf::points() const
{
  return{
    std::tuple<double, double, double>(std::get<0>(_p0), std::get<1>(_p0), std::get<2>(_p0)),
    std::tuple<double, double, double>(std::get<0>(_p1), std::get<1>(_p1), std::get<2>(_p1)),
    std::tuple<double, double, double>(std::get<0>(_p2), std::get<1>(_p2), std::get<2>(_p2))
  };
}

size_t			RT::TriangleCsgLeaf::build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> & primitives, Math::Matrix<4, 4> const & transformation, RT::Material const & material, unsigned int deph) const
{
  primitives.push_back(RT::OpenCL::Primitive());

  Math::Matrix<4, 4>		matrix = transformation.inverse();

  for (unsigned row = 0; row < 4; row++)
    for (unsigned col = 0; col < 4; col++)
      primitives.back().transformation[row * 4 + col] = (float)matrix(row, col);

  primitives.back().material.color[0] = (float)material.color.r;
  primitives.back().material.color[1] = (float)material.color.g;
  primitives.back().material.color[2] = (float)material.color.b;

  primitives.back().type = RT::OpenCL::Primitive::Type::PrimitiveTriangle;

  primitives.back().data.triangle.p[0][0] = (float)std::get<0>(_p0);
  primitives.back().data.triangle.p[0][1] = (float)std::get<1>(_p0);
  primitives.back().data.triangle.p[0][2] = (float)std::get<2>(_p0);

  primitives.back().data.triangle.p[1][0] = (float)std::get<0>(_p1);
  primitives.back().data.triangle.p[1][1] = (float)std::get<1>(_p1);
  primitives.back().data.triangle.p[1][2] = (float)std::get<2>(_p1);

  primitives.back().data.triangle.p[2][0] = (float)std::get<0>(_p2);
  primitives.back().data.triangle.p[2][1] = (float)std::get<1>(_p2);
  primitives.back().data.triangle.p[2][2] = (float)std::get<2>(_p2);

  primitives.back().data.triangle.v[0][0] = (float)_v0.x();
  primitives.back().data.triangle.v[0][1] = (float)_v0.y();
  primitives.back().data.triangle.v[0][2] = (float)_v0.z();

  primitives.back().data.triangle.v[1][0] = (float)_v1.x();
  primitives.back().data.triangle.v[1][1] = (float)_v1.y();
  primitives.back().data.triangle.v[1][2] = (float)_v1.z();

  primitives.back().data.triangle.n[0] = (float)_normal.x();
  primitives.back().data.triangle.n[1] = (float)_normal.y();
  primitives.back().data.triangle.n[2] = (float)_normal.z();

  primitives.back().data.triangle.index = 0;

  return primitives.size() - 1;
}