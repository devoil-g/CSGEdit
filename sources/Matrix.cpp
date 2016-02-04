#include "Matrix.hpp"

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::inverse() const
{
  Math::Matrix<4, 4>  matrix;
  double	      det, s0, s1, s2, s3, s4, s5, c0, c1, c2, c3, c4, c5;

  s0 = (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
  s1 = (*this)(0, 0) * (*this)(2, 1) - (*this)(0, 1) * (*this)(2, 0);
  s2 = (*this)(0, 0) * (*this)(3, 1) - (*this)(0, 1) * (*this)(3, 0);
  s3 = (*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0);
  s4 = (*this)(1, 0) * (*this)(3, 1) - (*this)(1, 1) * (*this)(3, 0);
  s5 = (*this)(2, 0) * (*this)(3, 1) - (*this)(2, 1) * (*this)(3, 0);

  c0 = (*this)(0, 2) * (*this)(1, 3) - (*this)(0, 3) * (*this)(1, 2);
  c1 = (*this)(0, 2) * (*this)(2, 3) - (*this)(0, 3) * (*this)(2, 2);
  c2 = (*this)(0, 2) * (*this)(3, 3) - (*this)(0, 3) * (*this)(3, 2);
  c3 = (*this)(1, 2) * (*this)(2, 3) - (*this)(1, 3) * (*this)(2, 2);
  c4 = (*this)(1, 2) * (*this)(3, 3) - (*this)(1, 3) * (*this)(3, 2);
  c5 = (*this)(2, 2) * (*this)(3, 3) - (*this)(2, 3) * (*this)(3, 2);

  det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

#ifdef _DEBUG
  // Not supposed to happen
  if (det == 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

  det = 1.f / det;

  matrix(0, 0) = (+(*this)(1, 1) * c5 - (*this)(2, 1) * c4 + (*this)(3, 1) * c3) * det;
  matrix(1, 0) = (-(*this)(1, 0) * c5 + (*this)(2, 0) * c4 - (*this)(3, 0) * c3) * det;
  matrix(2, 0) = (+(*this)(1, 3) * s5 - (*this)(2, 3) * s4 + (*this)(3, 3) * s3) * det;
  matrix(3, 0) = (-(*this)(1, 2) * s5 + (*this)(2, 2) * s4 - (*this)(3, 2) * s3) * det;

  matrix(0, 1) = (-(*this)(0, 1) * c5 + (*this)(2, 1) * c2 - (*this)(3, 1) * c1) * det;
  matrix(1, 1) = (+(*this)(0, 0) * c5 - (*this)(2, 0) * c2 + (*this)(3, 0) * c1) * det;
  matrix(2, 1) = (-(*this)(0, 3) * s5 + (*this)(2, 3) * s2 - (*this)(3, 3) * s1) * det;
  matrix(3, 1) = (+(*this)(0, 2) * s5 - (*this)(2, 2) * s2 + (*this)(3, 2) * s1) * det;

  matrix(0, 2) = (+(*this)(0, 1) * c4 - (*this)(1, 1) * c2 + (*this)(3, 1) * c0) * det;
  matrix(1, 2) = (-(*this)(0, 0) * c4 + (*this)(1, 0) * c2 - (*this)(3, 0) * c0) * det;
  matrix(2, 2) = (+(*this)(0, 3) * s4 - (*this)(1, 3) * s2 + (*this)(3, 3) * s0) * det;
  matrix(3, 2) = (-(*this)(0, 2) * s4 + (*this)(1, 2) * s2 - (*this)(3, 2) * s0) * det;

  matrix(0, 3) = (-(*this)(0, 1) * c3 + (*this)(1, 1) * c1 - (*this)(2, 1) * c0) * det;
  matrix(1, 3) = (+(*this)(0, 0) * c3 - (*this)(1, 0) * c1 + (*this)(2, 0) * c0) * det;
  matrix(2, 3) = (-(*this)(0, 3) * s3 + (*this)(1, 3) * s1 - (*this)(2, 3) * s0) * det;
  matrix(3, 3) = (+(*this)(0, 2) * s3 - (*this)(1, 2) * s1 + (*this)(2, 2) * s0) * det;

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::translation(double x, double y)
{
  Math::Matrix<3, 3>	matrix = Math::Matrix<3, 3>::identite();

  // Generate a translation matrix
  matrix(0, 2) = x;
  matrix(1, 2) = y;

  return matrix;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::translation(double x, double y, double z)
{
  Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::identite();

  // Generate a translation matrix
  matrix(0, 3) = x;
  matrix(1, 3) = y;
  matrix(2, 3) = z;

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::reflection(double x, double y)
{
  Math::Matrix<3, 3>  matrix = Math::Matrix<3, 3>::identite();

  // Generate a reflection matrix
  matrix(0, 0) = 1 - 2 * x * x;
  matrix(1, 0) = -2 * x * y;
  matrix(0, 1) = -2 * y * x;
  matrix(1, 1) = 1 - 2 * y * y;
  
  return matrix;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::reflection(double x, double y, double z)
{
  Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::identite();

  // Generate a reflection matrix
  matrix(0, 0) = 1 - 2 * x * x;
  matrix(1, 0) = -2 * x * y;
  matrix(2, 0) = -2 * x * z;
  matrix(0, 1) = -2 * y * x;
  matrix(1, 1) = 1 - 2 * y * y;
  matrix(2, 1) = -2 * y * z;
  matrix(0, 2) = -2 * z * x;
  matrix(1, 2) = -2 * z * y;
  matrix(2, 2) = 1 - 2 * z * z;

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::rotation(double z)
{
  Math::Matrix<3, 3>	matrix = Math::Matrix<3, 3>::identite();
  
  // Convert parameter to radian
  z = Math::Utils::DegToRad(z);

  // Generate a reflection matrix
  matrix(0, 0) = +std::cos(z);
  matrix(1, 0) = +std::sin(z);
  matrix(0, 1) = -std::sin(z);
  matrix(1, 1) = +std::cos(z);

  return matrix;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::rotation(double x, double y, double z)
{
  Math::Matrix<4, 4>	m_x = Math::Matrix<4, 4>::identite();
  Math::Matrix<4, 4>	m_y = Math::Matrix<4, 4>::identite();
  Math::Matrix<4, 4>	m_z = Math::Matrix<4, 4>::identite();

  // Convert parameters to radian
  x = Math::Utils::DegToRad(x);
  y = Math::Utils::DegToRad(y);
  z = Math::Utils::DegToRad(z);

  // Generate rotation matrix
  m_x(1, 1) = +std::cos(x);
  m_x(1, 2) = -std::sin(x);
  m_x(2, 1) = +std::sin(x);
  m_x(2, 2) = +std::cos(x);
  m_y(0, 0) = +std::cos(y);
  m_y(0, 2) = +std::sin(y);
  m_y(2, 0) = -std::sin(y);
  m_y(2, 2) = +std::cos(y);
  m_z(0, 0) = +std::cos(z);
  m_z(0, 1) = -std::sin(z);
  m_z(1, 0) = +std::sin(z);
  m_z(1, 1) = +std::cos(z);

  return m_z * m_y * m_x;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::rotation(double a, double x, double y, double z)
{
  Math::Matrix<4, 4>  matrix = Math::Matrix<4, 4>::identite();
  double	      l;

  l = std::sqrt(x * x + y * y + z * z);

#ifdef _DEBUG
  // Should not happen
  if (l == 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

  // Normalize vector
  x /= l;
  y /= l;
  z /= l;

  // Convert input to radian
  a = Math::Utils::DegToRad(a);

  // Generate a rotation matrix
  matrix(0, 0) = x * x * (1 - std::cos(a)) + std::cos(a);
  matrix(1, 0) = x * y * (1 - std::cos(a)) + z * std::sin(a);
  matrix(2, 0) = x * z * (1 - std::cos(a)) - y * std::sin(a);
  matrix(0, 1) = y * x * (1 - std::cos(a)) - z * std::sin(a);
  matrix(1, 1) = y * y * (1 - std::cos(a)) + std::cos(a);
  matrix(2, 1) = y * z * (1 - std::cos(a)) + x * std::sin(a);
  matrix(0, 2) = z * x * (1 - std::cos(a)) + y * std::sin(a);
  matrix(1, 2) = z * y * (1 - std::cos(a)) - x * std::sin(a);
  matrix(2, 2) = z * z * (1 - std::cos(a)) + std::cos(a);

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::scale(double x, double y)
{
  Math::Matrix<3, 3>	matrix;

  // Generate a scale matrix
  matrix(0, 0) = x;
  matrix(1, 1) = y;
  matrix(2, 2) = 1.f;

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::scale(double s)
{
  return Math::Matrix<3, 3>::scale(s, s);
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::scale(double x, double y, double z)
{
  Math::Matrix<4, 4>	matrix;

  // Generate a scale matrix
  matrix(0, 0) = x;
  matrix(1, 1) = y;
  matrix(2, 2) = z;
  matrix(3, 3) = 1.f;

  return matrix;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::scale(double s)
{
  return Math::Matrix<4, 4>::scale(s, s, s);
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::shear(double xy, double xz, double yx, double yz, double zx, double zy)
{
  Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::identite();

  // Generate shearing matrix
  matrix(0, 1) = xy;
  matrix(0, 2) = xz;
  matrix(1, 0) = yx;
  matrix(1, 2) = yz;
  matrix(2, 0) = zx;
  matrix(2, 1) = zy;

  return matrix;
}
