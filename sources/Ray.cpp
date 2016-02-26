#include "Math.hpp"
#include "Ray.hpp"

double		RT::Ray::cos(RT::Ray const & A, RT::Ray const & B)
{
  // Get squared vector lenght
  double	v1 = A.d().x() * A.d().x() + A.d().y() * A.d().y() + A.d().z() * A.d().z();
  double	v2 = B.d().x() * B.d().x() + B.d().y() * B.d().y() + B.d().z() * B.d().z();

#ifdef _DEBUG
  // Should not happen
  if (v1 == 0 || v2 == 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

  // Get lenght of A+B
  double	v12 = (A.d().x() + B.d().x()) * (A.d().x() + B.d().x())
    + (A.d().y() + B.d().y()) * (A.d().y() + B.d().y())
    + (A.d().z() + B.d().z()) * (A.d().z() + B.d().z());

  return (v12 - (v1 + v2)) / (2.f * std::sqrt(v1 * v2));
}

double		RT::Ray::angle(RT::Ray const & A, RT::Ray const & B)
{
  return std::acos(RT::Ray::cos(A, B));
}

double		RT::Ray::scalaire(RT::Ray const & A, RT::Ray const & B)
{
  return ((A.d().x() * A.d().x() + A.d().y() * A.d().y() + A.d().z() * A.d().z())
    + (B.d().x() * B.d().x() + B.d().y() * B.d().y() + B.d().z() * B.d().z())
    - ((A.d().x() - B.d().x()) * (A.d().x() - B.d().x()) + (A.d().y() - B.d().y()) * (A.d().y() - B.d().y()) + (A.d().z() - B.d().z()) * (A.d().z() - B.d().z())))
    / 2.f;
}

RT::Ray		RT::Ray::vectoriel(RT::Ray const & A, RT::Ray const & B)
{
  RT::Ray	ray;

  // Apply cross product
  ray.d().x() = A.d().y() * B.d().z() - A.d().z() * B.d().y();
  ray.d().y() = A.d().z() * B.d().x() - A.d().x() * B.d().z();
  ray.d().z() = A.d().x() * B.d().y() - A.d().y() * B.d().x();

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
