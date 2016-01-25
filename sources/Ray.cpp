#include "Ray.hpp"

Math::Ray::Ray()
{
  _p(3, 0) = 1.f;
  _d(3, 0) = 1.f;
}

Math::Ray::~Ray()
{}

double		Math::Ray::cos(Math::Ray const & A, Math::Ray const & B)
{
  double	v1, v2, v12;

  // Get squared vector lenght
  v1 = A.dx() * A.dx() + A.dy() * A.dy() + A.dz() * A.dz();
  v2 = B.dx() * B.dx() + B.dy() * B.dy() + B.dz() * B.dz();

#ifdef _DEBUG
  // Should not happen
  if (v1 == 0 || v2 == 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

  // Get lenght of A+B
  v12 = (A.dx() + B.dx()) * (A.dx() + B.dx())
    + (A.dy() + B.dy()) * (A.dy() + B.dy())
    + (A.dz() + B.dz()) * (A.dz() + B.dz());

  return (v12 - (v1 + v2)) / (2 * sqrt(v1 * v2));
}

double		Math::Ray::angle(Math::Ray const & A, Math::Ray const & B)
{
  return acos(Math::Ray::cos(A, B));
}

double		Math::Ray::scalaire(Math::Ray const & A, Math::Ray const & B)
{
  double	v1, v2, v12;

  // Get squared vector lenght
  v1 = A.dx() * A.dx() + A.dy() * A.dy() + A.dz() * A.dz();
  v2 = B.dx() * B.dx() + B.dy() * B.dy() + B.dz() * B.dz();

  // Check if v1 or v2 null
  if (v1 == 0 || v2 == 0)
    return 0;

  // Get lenght of A+B
  v12 = (A.dx() + B.dx()) * (A.dx() + B.dx())
    + (A.dy() + B.dy()) * (A.dy() + B.dy())
    + (A.dz() + B.dz()) * (A.dz() + B.dz());

  return (v12 - (v1 + v2)) / sqrt(v1 * v2);
}

Math::Ray	Math::Ray::vectoriel(Math::Ray const & A, Math::Ray const & B)
{
  Math::Ray	ray;

  // Apply cross product
  ray.dx() = A.dy() * B.dz() - A.dz() * B.dy();
  ray.dy() = A.dz() * B.dx() - A.dx() * B.dz();
  ray.dz() = A.dx() * B.dy() - A.dy() * B.dx();

  return ray;
}

Math::Ray	Math::Ray::normalize() const
{
  Math::Ray	ray;
  double	l;

  // Get lenght of ray
  l = sqrt(+this->dx() * this->dx() + this->dy() * this->dy() + this->dz() * this->dz());

#ifdef _DEBUG
  // Should not happen
  if (l == 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

  ray.p() = this->p();

  // Scale ray according to his lenght
  ray.dx() = this->dx() / l;
  ray.dy() = this->dy() / l;
  ray.dz() = this->dz() / l;

  return ray;
}

Math::Ray	operator*(Math::Matrix<4, 4> const & mat, Math::Ray const & ray)
{
  Math::Matrix<4, 4>  matrix(mat);
  Math::Ray	      r;

  // Apply transformation matrix to position
  r.p() = matrix * ray.p();

  // Ignore translation for direction vector
  matrix(0, 3) = 0;
  matrix(1, 3) = 0;
  matrix(2, 3) = 0;
  r.d() = matrix * ray.d();

  return r;
}
