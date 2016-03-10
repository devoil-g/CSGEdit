#include "Math.hpp"
#include "Ray.hpp"

double		RT::Ray::cos(Math::Vector<4> const & A, Math::Vector<4> const & B)
{
  // Get squared vector lenght
  double	v1 = A.x() * A.x() + A.y() * A.y() + A.z() * A.z();
  double	v2 = B.x() * B.x() + B.y() * B.y() + B.z() * B.z();

#ifdef _DEBUG
  // Should not happen
  if (v1 == 0 || v2 == 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

  // Get lenght of A+B
  double	v12 = (A.x() + B.x()) * (A.x() + B.x())
    + (A.y() + B.y()) * (A.y() + B.y())
    + (A.z() + B.z()) * (A.z() + B.z());

  return (v12 - (v1 + v2)) / (2.f * std::sqrt(v1 * v2));
}

double		RT::Ray::angle(Math::Vector<4> const & A, Math::Vector<4> const & B)
{
  return std::acos(RT::Ray::cos(A, B));
}

double		RT::Ray::scalaire(Math::Vector<4> const & A, Math::Vector<4> const & B)
{
  return ((A.x() * A.x() + A.y() * A.y() + A.z() * A.z())
    + (B.x() * B.x() + B.y() * B.y() + B.z() * B.z())
    - ((A.x() - B.x()) * (A.x() - B.x()) + (A.y() - B.y()) * (A.y() - B.y()) + (A.z() - B.z()) * (A.z() - B.z())))
    / 2.f;
}

Math::Vector<4>	RT::Ray::vectoriel(Math::Vector<4> const & A, Math::Vector<4> const & B)
{
  Math::Vector<4>	ray;

  // Apply cross product
  ray.x() = A.y() * B.z() - A.z() * B.y();
  ray.y() = A.z() * B.x() - A.x() * B.z();
  ray.z() = A.x() * B.y() - A.y() * B.x();

  return ray;
}

RT::Ray		RT::Ray::normalize() const
{
  RT::Ray	r(*this);

  r.d() /= std::sqrt(_d.x() * _d.x() + _d.y() * _d.y() + _d.z() * _d.z());
  r.p()(3) = 1.f;
  r.d()(3) = 0.f;

  return r;
}

RT::Ray		operator*(Math::Matrix<4, 4> const & matrix, RT::Ray const & ray)
{
  RT::Ray	r;

  r.p() = matrix * ray.p();
  r.d() = matrix * ray.d();

  return r;
}
