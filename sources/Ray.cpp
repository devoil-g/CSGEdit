#include "Ray.hpp"

double		RT::Ray::cos(Math::Vector<4> const & A, Math::Vector<4> const & B)
{
#ifdef _DEBUG
  // Should not happen
  if (A.x() * A.x() + A.y() * A.y() + A.z() * A.z() == 0 || B.x() * B.x() + B.y() * B.y() + B.z() * B.z() == 0)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
#endif

  return RT::Ray::scalaire(A, B) / std::sqrt((A.x() * A.x() + A.y() * A.y() + A.z() * A.z()) * (B.x() * B.x() + B.y() * B.y() + B.z() * B.z()));
}

double		RT::Ray::angle(Math::Vector<4> const & A, Math::Vector<4> const & B)
{
  return std::acos(RT::Ray::cos(A, B));
}

double		RT::Ray::scalaire(Math::Vector<4> const & A, Math::Vector<4> const & B)
{
  return A.x() * B.x() + A.y() * B.y() + A.z() * B.z();
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

  r.d() /= length();
  r.p()(3) = 1.f;
  r.d()(3) = 0.f;

  return r;
}

double		RT::Ray::length() const
{
  return std::sqrt(_d.x() * _d.x() + _d.y() * _d.y() + _d.z() * _d.z());
}

RT::Ray		operator*(Math::Matrix<4, 4> const & matrix, RT::Ray const & ray)
{
  RT::Ray	r;

  r.p() = matrix * ray.p();
  r.d() = matrix * ray.d();

  return r;
}
