#include "Matrix.hpp"

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::inverse() const
{
  Math::Matrix<4, 4>  matrix;
  double	      det, s0, s1, s2, s3, s4, s5, c0, c1, c2, c3, c4, c5;

  s0 = this->_matrix[0][0] * this->_matrix[1][1] - this->_matrix[1][0] * this->_matrix[0][1];
  s1 = this->_matrix[0][0] * this->_matrix[1][2] - this->_matrix[1][0] * this->_matrix[0][2];
  s2 = this->_matrix[0][0] * this->_matrix[1][3] - this->_matrix[1][0] * this->_matrix[0][3];
  s3 = this->_matrix[0][1] * this->_matrix[1][2] - this->_matrix[1][1] * this->_matrix[0][2];
  s4 = this->_matrix[0][1] * this->_matrix[1][3] - this->_matrix[1][1] * this->_matrix[0][3];
  s5 = this->_matrix[0][2] * this->_matrix[1][3] - this->_matrix[1][2] * this->_matrix[0][3];

  c0 = this->_matrix[2][0] * this->_matrix[3][1] - this->_matrix[3][0] * this->_matrix[2][1];
  c1 = this->_matrix[2][0] * this->_matrix[3][2] - this->_matrix[3][0] * this->_matrix[2][2];
  c2 = this->_matrix[2][0] * this->_matrix[3][3] - this->_matrix[3][0] * this->_matrix[2][3];
  c3 = this->_matrix[2][1] * this->_matrix[3][2] - this->_matrix[3][1] * this->_matrix[2][2];
  c4 = this->_matrix[2][1] * this->_matrix[3][3] - this->_matrix[3][1] * this->_matrix[2][3];
  c5 = this->_matrix[2][2] * this->_matrix[3][3] - this->_matrix[3][2] * this->_matrix[2][3];

  det = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

#ifdef _DEBUG
  // Not supposed to happen
  if (det == 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

  det = 1.f / det;

  matrix._matrix[0][0] =
    (+this->_matrix[1][1] * c5 - this->_matrix[1][2] * c4 + this->_matrix[1][3] * c3) * det;
  matrix._matrix[0][1] =
    (-this->_matrix[0][1] * c5 + this->_matrix[0][2] * c4 - this->_matrix[0][3] * c3) * det;
  matrix._matrix[0][2] =
    (+this->_matrix[3][1] * s5 - this->_matrix[3][2] * s4 + this->_matrix[3][3] * s3) * det;
  matrix._matrix[0][3] =
    (-this->_matrix[2][1] * s5 + this->_matrix[2][2] * s4 - this->_matrix[2][3] * s3) * det;

  matrix._matrix[1][0] =
    (-this->_matrix[1][0] * c5 + this->_matrix[1][2] * c2 - this->_matrix[1][3] * c1) * det;
  matrix._matrix[1][1] =
    (+this->_matrix[0][0] * c5 - this->_matrix[0][2] * c2 + this->_matrix[0][3] * c1) * det;
  matrix._matrix[1][2] =
    (-this->_matrix[3][0] * s5 + this->_matrix[3][2] * s2 - this->_matrix[3][3] * s1) * det;
  matrix._matrix[1][3] =
    (+this->_matrix[2][0] * s5 - this->_matrix[2][2] * s2 + this->_matrix[2][3] * s1) * det;

  matrix._matrix[2][0] =
    (+this->_matrix[1][0] * c4 - this->_matrix[1][1] * c2 + this->_matrix[1][3] * c0) * det;
  matrix._matrix[2][1] =
    (-this->_matrix[0][0] * c4 + this->_matrix[0][1] * c2 - this->_matrix[0][3] * c0) * det;
  matrix._matrix[2][2] =
    (+this->_matrix[3][0] * s4 - this->_matrix[3][1] * s2 + this->_matrix[3][3] * s0) * det;
  matrix._matrix[2][3] =
    (-this->_matrix[2][0] * s4 + this->_matrix[2][1] * s2 - this->_matrix[2][3] * s0) * det;

  matrix._matrix[3][0] =
    (-this->_matrix[1][0] * c3 + this->_matrix[1][1] * c1 - this->_matrix[1][2] * c0) * det;
  matrix._matrix[3][1] =
    (+this->_matrix[0][0] * c3 - this->_matrix[0][1] * c1 + this->_matrix[0][2] * c0) * det;
  matrix._matrix[3][2] =
    (-this->_matrix[3][0] * s3 + this->_matrix[3][1] * s1 - this->_matrix[3][2] * s0) * det;
  matrix._matrix[3][3] =
    (+this->_matrix[2][0] * s3 - this->_matrix[2][1] * s1 + this->_matrix[2][2] * s0) * det;

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::transpose() const
{
  Math::Matrix<3, 3>  matrix;

  matrix._matrix[0][0] = this->_matrix[0][0];
  matrix._matrix[0][1] = this->_matrix[1][0];
  matrix._matrix[0][2] = this->_matrix[2][0];

  matrix._matrix[1][0] = this->_matrix[0][1];
  matrix._matrix[1][1] = this->_matrix[1][1];
  matrix._matrix[1][2] = this->_matrix[2][1];

  matrix._matrix[2][0] = this->_matrix[0][2];
  matrix._matrix[2][1] = this->_matrix[1][2];
  matrix._matrix[2][2] = this->_matrix[2][2];

  return matrix;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::transpose() const
{
  Math::Matrix<4, 4>  matrix;

  matrix._matrix[0][0] = this->_matrix[0][0];
  matrix._matrix[0][1] = this->_matrix[1][0];
  matrix._matrix[0][2] = this->_matrix[2][0];
  matrix._matrix[0][3] = this->_matrix[3][0];

  matrix._matrix[1][0] = this->_matrix[0][1];
  matrix._matrix[1][1] = this->_matrix[1][1];
  matrix._matrix[1][2] = this->_matrix[2][1];
  matrix._matrix[1][3] = this->_matrix[3][1];

  matrix._matrix[2][0] = this->_matrix[0][2];
  matrix._matrix[2][1] = this->_matrix[1][2];
  matrix._matrix[2][2] = this->_matrix[2][2];
  matrix._matrix[2][3] = this->_matrix[3][2];

  matrix._matrix[3][0] = this->_matrix[0][3];
  matrix._matrix[3][1] = this->_matrix[1][3];
  matrix._matrix[3][2] = this->_matrix[2][3];
  matrix._matrix[3][3] = this->_matrix[3][3];

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::identite()
{
  Math::Matrix<3, 3>  matrix;

  matrix._matrix[0][0] = 1.f;
  matrix._matrix[1][1] = 1.f;
  matrix._matrix[2][2] = 1.f;

  return matrix;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::identite()
{
  Math::Matrix<4, 4>  matrix;

  matrix._matrix[0][0] = 1.f;
  matrix._matrix[1][1] = 1.f;
  matrix._matrix[2][2] = 1.f;
  matrix._matrix[3][3] = 1.f;

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::translation(double x, double y)
{
  Math::Matrix<3, 3>	matrix = Math::Matrix<3, 3>::identite();

  // Generate a translation matrix
  matrix._matrix[2][0] = x;
  matrix._matrix[2][1] = y;

  return matrix;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::translation(double x, double y, double z)
{
  Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::identite();

  // Generate a translation matrix
  matrix._matrix[3][0] = x;
  matrix._matrix[3][1] = y;
  matrix._matrix[3][2] = z;

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::reflection(double x, double y)
{
  Math::Matrix<3, 3>  matrix = Math::Matrix<3, 3>::identite();

  // Generate a reflection matrix
  matrix._matrix[0][0] = 1 - 2 * x * x;
  matrix._matrix[0][1] = -2 * x * y;
  matrix._matrix[1][0] = -2 * y * x;
  matrix._matrix[1][1] = 1 - 2 * y * y;
  
  return matrix;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::reflection(double x, double y, double z)
{
  Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::identite();

  // Generate a reflection matrix
  matrix._matrix[0][0] = 1 - 2 * x * x;
  matrix._matrix[0][1] = -2 * x * y;
  matrix._matrix[0][2] = -2 * x * z;
  matrix._matrix[1][0] = -2 * y * x;
  matrix._matrix[1][1] = 1 - 2 * y * y;
  matrix._matrix[1][2] = -2 * y * z;
  matrix._matrix[2][0] = -2 * z * x;
  matrix._matrix[2][1] = -2 * z * y;
  matrix._matrix[2][2] = 1 - 2 * z * z;

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::rotation(double z)
{
  Math::Matrix<3, 3>	matrix = Math::Matrix<3, 3>::identite();
  
  // Convert parameter to radian
  z = Math::Utils::DegToRad(z);

  // Generate a reflection matrix
  matrix._matrix[0][0] = +std::cos(z);
  matrix._matrix[0][1] = +std::sin(z);
  matrix._matrix[1][0] = -std::sin(z);
  matrix._matrix[1][1] = +std::cos(z);

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
  m_x._matrix[1][1] = +std::cos(x);
  m_x._matrix[2][1] = -std::sin(x);
  m_x._matrix[1][2] = +std::sin(x);
  m_x._matrix[2][2] = +std::cos(x);
  m_y._matrix[0][0] = +std::cos(y);
  m_y._matrix[2][0] = +std::sin(y);
  m_y._matrix[0][2] = -std::sin(y);
  m_y._matrix[2][2] = +std::cos(y);
  m_z._matrix[0][0] = +std::cos(z);
  m_z._matrix[1][0] = -std::sin(z);
  m_z._matrix[0][1] = +std::sin(z);
  m_z._matrix[1][1] = +std::cos(z);

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
  matrix._matrix[0][0] = x * x * (1 - std::cos(a)) + std::cos(a);
  matrix._matrix[0][1] = x * y * (1 - std::cos(a)) + z * std::sin(a);
  matrix._matrix[0][2] = x * z * (1 - std::cos(a)) - y * std::sin(a);
  matrix._matrix[1][0] = y * x * (1 - std::cos(a)) - z * std::sin(a);
  matrix._matrix[1][1] = y * y * (1 - std::cos(a)) + std::cos(a);
  matrix._matrix[1][2] = y * z * (1 - std::cos(a)) + x * std::sin(a);
  matrix._matrix[2][0] = z * x * (1 - std::cos(a)) + y * std::sin(a);
  matrix._matrix[2][1] = z * y * (1 - std::cos(a)) - x * std::sin(a);
  matrix._matrix[2][2] = z * z * (1 - std::cos(a)) + std::cos(a);

  return matrix;
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::scale(double s)
{
  return Math::Matrix<3, 3>::scale(s, s);
}

template<>
Math::Matrix<3, 3>    Math::Matrix<3, 3>::scale(double x, double y)
{
  Math::Matrix<3, 3>	matrix;

  // Generate a scale matrix
  matrix._matrix[0][0] = x;
  matrix._matrix[1][1] = y;
  matrix._matrix[2][2] = 1.f;

  return matrix;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::scale(double s)
{
  return Math::Matrix<4, 4>::scale(s, s, s);
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::scale(double x, double y, double z)
{
  Math::Matrix<4, 4>	matrix;

  // Generate a scale matrix
  matrix._matrix[0][0] = x;
  matrix._matrix[1][1] = y;
  matrix._matrix[2][2] = z;
  matrix._matrix[3][3] = 1.f;

  return matrix;
}

template<>
Math::Matrix<4, 4>    Math::Matrix<4, 4>::shear(double xy, double xz, double yx, double yz, double zx, double zy)
{
  Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::identite();

  // Generate shearing matrix
  matrix._matrix[1][0] = xy;
  matrix._matrix[2][0] = xz;
  matrix._matrix[0][1] = yx;
  matrix._matrix[2][1] = yz;
  matrix._matrix[0][2] = zx;
  matrix._matrix[1][2] = zy;

  return matrix;
}

// ------------------------------------------------------------------------------------------

Math::Matrix<4, 4>    operator*(Math::Matrix<4, 4> const & A, Math::Matrix<4, 4> const & B)
{
  Math::Matrix<4, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2) + A(3, 2) * B(0, 3);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1) + A(2, 3) * B(0, 2) + A(3, 3) * B(0, 3);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2) + A(3, 1) * B(1, 3);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2) + A(3, 2) * B(1, 3);
  matrix(1, 3) = A(0, 3) * B(1, 0) + A(1, 3) * B(1, 1) + A(2, 3) * B(1, 2) + A(3, 3) * B(1, 3);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2) + A(3, 0) * B(2, 3);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2) + A(3, 1) * B(2, 3);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2) + A(3, 2) * B(2, 3);
  matrix(2, 3) = A(0, 3) * B(2, 0) + A(1, 3) * B(2, 1) + A(2, 3) * B(2, 2) + A(3, 3) * B(2, 3);

  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1) + A(2, 0) * B(3, 2) + A(3, 0) * B(3, 3);
  matrix(3, 1) = A(0, 1) * B(3, 0) + A(1, 1) * B(3, 1) + A(2, 1) * B(3, 2) + A(3, 1) * B(3, 3);
  matrix(3, 2) = A(0, 2) * B(3, 0) + A(1, 2) * B(3, 1) + A(2, 2) * B(3, 2) + A(3, 2) * B(3, 3);
  matrix(3, 3) = A(0, 3) * B(3, 0) + A(1, 3) * B(3, 1) + A(2, 3) * B(3, 2) + A(3, 3) * B(3, 3);

  return matrix;
}

Math::Matrix<3, 4>    operator*(Math::Matrix<4, 4> const & A, Math::Matrix<3, 4> const & B)
{
  Math::Matrix<3, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2) + A(3, 2) * B(0, 3);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1) + A(2, 3) * B(0, 2) + A(3, 3) * B(0, 3);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2) + A(3, 1) * B(1, 3);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2) + A(3, 2) * B(1, 3);
  matrix(1, 3) = A(0, 3) * B(1, 0) + A(1, 3) * B(1, 1) + A(2, 3) * B(1, 2) + A(3, 3) * B(1, 3);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2) + A(3, 0) * B(2, 3);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2) + A(3, 1) * B(2, 3);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2) + A(3, 2) * B(2, 3);
  matrix(2, 3) = A(0, 3) * B(2, 0) + A(1, 3) * B(2, 1) + A(2, 3) * B(2, 2) + A(3, 3) * B(2, 3);

  return matrix;
}

Math::Matrix<2, 4>    operator*(Math::Matrix<4, 4> const & A, Math::Matrix<2, 4> const & B)
{
  Math::Matrix<2, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2) + A(3, 2) * B(0, 3);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1) + A(2, 3) * B(0, 2) + A(3, 3) * B(0, 3);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2) + A(3, 1) * B(1, 3);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2) + A(3, 2) * B(1, 3);
  matrix(1, 3) = A(0, 3) * B(1, 0) + A(1, 3) * B(1, 1) + A(2, 3) * B(1, 2) + A(3, 3) * B(1, 3);

  return matrix;
}

Math::Matrix<1, 4>    operator*(Math::Matrix<4, 4> const & A, Math::Matrix<1, 4> const & B)
{
  Math::Matrix<1, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2) + A(3, 2) * B(0, 3);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1) + A(2, 3) * B(0, 2) + A(3, 3) * B(0, 3);

  return matrix;
}



Math::Matrix<4, 3>    operator*(Math::Matrix<4, 3> const & A, Math::Matrix<4, 4> const & B)
{
  Math::Matrix<4, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2) + A(3, 2) * B(0, 3);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2) + A(3, 1) * B(1, 3);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2) + A(3, 2) * B(1, 3);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2) + A(3, 0) * B(2, 3);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2) + A(3, 1) * B(2, 3);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2) + A(3, 2) * B(2, 3);

  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1) + A(2, 0) * B(3, 2) + A(3, 0) * B(3, 3);
  matrix(3, 1) = A(0, 1) * B(3, 0) + A(1, 1) * B(3, 1) + A(2, 1) * B(3, 2) + A(3, 1) * B(3, 3);
  matrix(3, 2) = A(0, 2) * B(3, 0) + A(1, 2) * B(3, 1) + A(2, 2) * B(3, 2) + A(3, 2) * B(3, 3);

  return matrix;
}

Math::Matrix<3, 3>    operator*(Math::Matrix<4, 3> const & A, Math::Matrix<3, 4> const & B)
{
  Math::Matrix<3, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2) + A(3, 2) * B(0, 3);
  
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2) + A(3, 1) * B(1, 3);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2) + A(3, 2) * B(1, 3);
  
  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2) + A(3, 0) * B(2, 3);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2) + A(3, 1) * B(2, 3);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2) + A(3, 2) * B(2, 3);
  
  return matrix;
}

Math::Matrix<2, 3>    operator*(Math::Matrix<4, 3> const & A, Math::Matrix<2, 4> const & B)
{
  Math::Matrix<2, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2) + A(3, 2) * B(0, 3);
  
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2) + A(3, 1) * B(1, 3);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2) + A(3, 2) * B(1, 3);
  
  return matrix;
}

Math::Matrix<1, 3>    operator*(Math::Matrix<4, 3> const & A, Math::Matrix<1, 4> const & B)
{
  Math::Matrix<1, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2) + A(3, 2) * B(0, 3);
  
  return matrix;
}



Math::Matrix<4, 2>    operator*(Math::Matrix<4, 2> const & A, Math::Matrix<4, 4> const & B)
{
  Math::Matrix<4, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2) + A(3, 1) * B(1, 3);
  
  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2) + A(3, 0) * B(2, 3);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2) + A(3, 1) * B(2, 3);
  
  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1) + A(2, 0) * B(3, 2) + A(3, 0) * B(3, 3);
  matrix(3, 1) = A(0, 1) * B(3, 0) + A(1, 1) * B(3, 1) + A(2, 1) * B(3, 2) + A(3, 1) * B(3, 3);
  
  return matrix;
}

Math::Matrix<3, 2>    operator*(Math::Matrix<4, 2> const & A, Math::Matrix<3, 4> const & B)
{
  Math::Matrix<3, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2) + A(3, 1) * B(1, 3);
  
  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2) + A(3, 0) * B(2, 3);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2) + A(3, 1) * B(2, 3);
  
  return matrix;
}

Math::Matrix<2, 2>    operator*(Math::Matrix<4, 2> const & A, Math::Matrix<2, 4> const & B)
{
  Math::Matrix<2, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2) + A(3, 1) * B(1, 3);
  
  return matrix;
}

Math::Matrix<1, 2>    operator*(Math::Matrix<4, 2> const & A, Math::Matrix<1, 4> const & B)
{
  Math::Matrix<1, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2) + A(3, 1) * B(0, 3);
  
  return matrix;
}



Math::Matrix<4, 1>    operator*(Math::Matrix<4, 1> const & A, Math::Matrix<4, 4> const & B)
{
  Math::Matrix<4, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2) + A(3, 0) * B(2, 3);
  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1) + A(2, 0) * B(3, 2) + A(3, 0) * B(3, 3);
  
  return matrix;
}

Math::Matrix<3, 1>    operator*(Math::Matrix<4, 1> const & A, Math::Matrix<3, 4> const & B)
{
  Math::Matrix<3, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);
  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2) + A(3, 0) * B(2, 3);
  
  return matrix;
}

Math::Matrix<2, 1>    operator*(Math::Matrix<4, 1> const & A, Math::Matrix<2, 4> const & B)
{
  Math::Matrix<2, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2) + A(3, 0) * B(1, 3);

  return matrix;
}

Math::Matrix<1, 1>    operator*(Math::Matrix<4, 1> const & A, Math::Matrix<1, 4> const & B)
{
  Math::Matrix<1, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2) + A(3, 0) * B(0, 3);
  
  return matrix;
}

// ------------------------------------------------------------------------------------------

Math::Matrix<4, 4>    operator*(Math::Matrix<3, 4> const & A, Math::Matrix<4, 3> const & B)
{
  Math::Matrix<4, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1) + A(2, 3) * B(0, 2);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2);
  matrix(1, 3) = A(0, 3) * B(1, 0) + A(1, 3) * B(1, 1) + A(2, 3) * B(1, 2);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2);
  matrix(2, 3) = A(0, 3) * B(2, 0) + A(1, 3) * B(2, 1) + A(2, 3) * B(2, 2);

  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1) + A(2, 0) * B(3, 2);
  matrix(3, 1) = A(0, 1) * B(3, 0) + A(1, 1) * B(3, 1) + A(2, 1) * B(3, 2);
  matrix(3, 2) = A(0, 2) * B(3, 0) + A(1, 2) * B(3, 1) + A(2, 2) * B(3, 2);
  matrix(3, 3) = A(0, 3) * B(3, 0) + A(1, 3) * B(3, 1) + A(2, 3) * B(3, 2);

  return matrix;
}

Math::Matrix<3, 4>    operator*(Math::Matrix<3, 4> const & A, Math::Matrix<3, 3> const & B)
{
  Math::Matrix<3, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1) + A(2, 3) * B(0, 2);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2);
  matrix(1, 3) = A(0, 3) * B(1, 0) + A(1, 3) * B(1, 1) + A(2, 3) * B(1, 2);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2);
  matrix(2, 3) = A(0, 3) * B(2, 0) + A(1, 3) * B(2, 1) + A(2, 3) * B(2, 2);

  return matrix;
}

Math::Matrix<2, 4>    operator*(Math::Matrix<3, 4> const & A, Math::Matrix<2, 3> const & B)
{
  Math::Matrix<2, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1) + A(2, 3) * B(0, 2);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2);
  matrix(1, 3) = A(0, 3) * B(1, 0) + A(1, 3) * B(1, 1) + A(2, 3) * B(1, 2);

  return matrix;
}

Math::Matrix<1, 4>    operator*(Math::Matrix<3, 4> const & A, Math::Matrix<1, 3> const & B)
{
  Math::Matrix<1, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1) + A(2, 3) * B(0, 2);

  return matrix;
}



Math::Matrix<4, 3>    operator*(Math::Matrix<3, 3> const & A, Math::Matrix<4, 3> const & B)
{
  Math::Matrix<4, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2);

  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1) + A(2, 0) * B(3, 2);
  matrix(3, 1) = A(0, 1) * B(3, 0) + A(1, 1) * B(3, 1) + A(2, 1) * B(3, 2);
  matrix(3, 2) = A(0, 2) * B(3, 0) + A(1, 2) * B(3, 1) + A(2, 2) * B(3, 2);

  return matrix;
}

Math::Matrix<3, 3>    operator*(Math::Matrix<3, 3> const & A, Math::Matrix<3, 3> const & B)
{
  Math::Matrix<3, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1) + A(2, 2) * B(2, 2);

  return matrix;
}

Math::Matrix<2, 3>    operator*(Math::Matrix<3, 3> const & A, Math::Matrix<2, 3> const & B)
{
  Math::Matrix<2, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1) + A(2, 2) * B(1, 2);

  return matrix;
}

Math::Matrix<1, 3>    operator*(Math::Matrix<3, 3> const & A, Math::Matrix<1, 3> const & B)
{
  Math::Matrix<1, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1) + A(2, 2) * B(0, 2);

  return matrix;
}



Math::Matrix<4, 2>    operator*(Math::Matrix<3, 2> const & A, Math::Matrix<4, 3> const & B)
{
  Math::Matrix<4, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2);

  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1) + A(2, 0) * B(3, 2);
  matrix(3, 1) = A(0, 1) * B(3, 0) + A(1, 1) * B(3, 1) + A(2, 1) * B(3, 2);

  return matrix;
}

Math::Matrix<3, 2>    operator*(Math::Matrix<3, 2> const & A, Math::Matrix<3, 3> const & B)
{
  Math::Matrix<3, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1) + A(2, 1) * B(2, 2);

  return matrix;
}

Math::Matrix<2, 2>    operator*(Math::Matrix<3, 2> const & A, Math::Matrix<2, 3> const & B)
{
  Math::Matrix<2, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1) + A(2, 1) * B(1, 2);

  return matrix;
}

Math::Matrix<1, 2>    operator*(Math::Matrix<3, 2> const & A, Math::Matrix<1, 3> const & B)
{
  Math::Matrix<1, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1) + A(2, 1) * B(0, 2);

  return matrix;
}



Math::Matrix<4, 1>    operator*(Math::Matrix<3, 1> const & A, Math::Matrix<4, 3> const & B)
{
  Math::Matrix<4, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2);
  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1) + A(2, 0) * B(3, 2);

  return matrix;
}

Math::Matrix<3, 1>    operator*(Math::Matrix<3, 1> const & A, Math::Matrix<3, 3> const & B)
{
  Math::Matrix<3, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);
  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1) + A(2, 0) * B(2, 2);

  return matrix;
}

Math::Matrix<2, 1>    operator*(Math::Matrix<3, 1> const & A, Math::Matrix<2, 3> const & B)
{
  Math::Matrix<2, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1) + A(2, 0) * B(1, 2);

  return matrix;
}

Math::Matrix<1, 1>    operator*(Math::Matrix<3, 1> const & A, Math::Matrix<1, 3> const & B)
{
  Math::Matrix<1, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1) + A(2, 0) * B(0, 2);

  return matrix;
}

// ------------------------------------------------------------------------------------------

Math::Matrix<4, 4>    operator*(Math::Matrix<2, 4> const & A, Math::Matrix<4, 2> const & B)
{
  Math::Matrix<4, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1);
  matrix(1, 3) = A(0, 3) * B(1, 0) + A(1, 3) * B(1, 1);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1);
  matrix(2, 3) = A(0, 3) * B(2, 0) + A(1, 3) * B(2, 1);

  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1);
  matrix(3, 1) = A(0, 1) * B(3, 0) + A(1, 1) * B(3, 1);
  matrix(3, 2) = A(0, 2) * B(3, 0) + A(1, 2) * B(3, 1);
  matrix(3, 3) = A(0, 3) * B(3, 0) + A(1, 3) * B(3, 1);

  return matrix;
}

Math::Matrix<3, 4>    operator*(Math::Matrix<2, 4> const & A, Math::Matrix<3, 2> const & B)
{
  Math::Matrix<3, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1);
  matrix(1, 3) = A(0, 3) * B(1, 0) + A(1, 3) * B(1, 1);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1);
  matrix(2, 3) = A(0, 3) * B(2, 0) + A(1, 3) * B(2, 1);

  return matrix;
}

Math::Matrix<2, 4>    operator*(Math::Matrix<2, 4> const & A, Math::Matrix<2, 2> const & B)
{
  Math::Matrix<2, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1);
  matrix(1, 3) = A(0, 3) * B(1, 0) + A(1, 3) * B(1, 1);

  return matrix;
}

Math::Matrix<1, 4>    operator*(Math::Matrix<2, 4> const & A, Math::Matrix<1, 2> const & B)
{
  Math::Matrix<1, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1);
  matrix(0, 3) = A(0, 3) * B(0, 0) + A(1, 3) * B(0, 1);

  return matrix;
}



Math::Matrix<4, 3>    operator*(Math::Matrix<2, 3> const & A, Math::Matrix<4, 2> const & B)
{
  Math::Matrix<4, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1);

  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1);
  matrix(3, 1) = A(0, 1) * B(3, 0) + A(1, 1) * B(3, 1);
  matrix(3, 2) = A(0, 2) * B(3, 0) + A(1, 2) * B(3, 1);

  return matrix;
}

Math::Matrix<3, 3>    operator*(Math::Matrix<2, 3> const & A, Math::Matrix<3, 2> const & B)
{
  Math::Matrix<3, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1);
  matrix(2, 2) = A(0, 2) * B(2, 0) + A(1, 2) * B(2, 1);

  return matrix;
}

Math::Matrix<2, 3>    operator*(Math::Matrix<2, 3> const & A, Math::Matrix<2, 2> const & B)
{
  Math::Matrix<2, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1);
  matrix(1, 2) = A(0, 2) * B(1, 0) + A(1, 2) * B(1, 1);

  return matrix;
}

Math::Matrix<1, 3>    operator*(Math::Matrix<2, 3> const & A, Math::Matrix<1, 2> const & B)
{
  Math::Matrix<1, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);
  matrix(0, 2) = A(0, 2) * B(0, 0) + A(1, 2) * B(0, 1);

  return matrix;
}



Math::Matrix<4, 2>    operator*(Math::Matrix<2, 2> const & A, Math::Matrix<4, 2> const & B)
{
  Math::Matrix<4, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1);

  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1);
  matrix(3, 1) = A(0, 1) * B(3, 0) + A(1, 1) * B(3, 1);

  return matrix;
}

Math::Matrix<3, 2>    operator*(Math::Matrix<2, 2> const & A, Math::Matrix<3, 2> const & B)
{
  Math::Matrix<3, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1);

  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1);
  matrix(2, 1) = A(0, 1) * B(2, 0) + A(1, 1) * B(2, 1);

  return matrix;
}

Math::Matrix<2, 2>    operator*(Math::Matrix<2, 2> const & A, Math::Matrix<2, 2> const & B)
{
  Math::Matrix<2, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);

  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(1, 1) = A(0, 1) * B(1, 0) + A(1, 1) * B(1, 1);

  return matrix;
}

Math::Matrix<1, 2>    operator*(Math::Matrix<2, 2> const & A, Math::Matrix<1, 2> const & B)
{
  Math::Matrix<1, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(0, 1) = A(0, 1) * B(0, 0) + A(1, 1) * B(0, 1);

  return matrix;
}



Math::Matrix<4, 1>    operator*(Math::Matrix<2, 1> const & A, Math::Matrix<4, 2> const & B)
{
  Math::Matrix<4, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1);
  matrix(3, 0) = A(0, 0) * B(3, 0) + A(1, 0) * B(3, 1);

  return matrix;
}

Math::Matrix<3, 1>    operator*(Math::Matrix<2, 1> const & A, Math::Matrix<3, 2> const & B)
{
  Math::Matrix<3, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);
  matrix(2, 0) = A(0, 0) * B(2, 0) + A(1, 0) * B(2, 1);

  return matrix;
}

Math::Matrix<2, 1>    operator*(Math::Matrix<2, 1> const & A, Math::Matrix<2, 2> const & B)
{
  Math::Matrix<2, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);
  matrix(1, 0) = A(0, 0) * B(1, 0) + A(1, 0) * B(1, 1);

  return matrix;
}

Math::Matrix<1, 1>    operator*(Math::Matrix<2, 1> const & A, Math::Matrix<1, 2> const & B)
{
  Math::Matrix<1, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0) + A(1, 0) * B(0, 1);

  return matrix;
}

// ------------------------------------------------------------------------------------------

Math::Matrix<4, 4>    operator*(Math::Matrix<1, 4> const & A, Math::Matrix<4, 1> const & B)
{
  Math::Matrix<4, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);
  matrix(0, 2) = A(0, 2) * B(0, 0);
  matrix(0, 3) = A(0, 3) * B(0, 0);

  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(1, 1) = A(0, 1) * B(1, 0);
  matrix(1, 2) = A(0, 2) * B(1, 0);
  matrix(1, 3) = A(0, 3) * B(1, 0);

  matrix(2, 0) = A(0, 0) * B(2, 0);
  matrix(2, 1) = A(0, 1) * B(2, 0);
  matrix(2, 2) = A(0, 2) * B(2, 0);
  matrix(2, 3) = A(0, 3) * B(2, 0);

  matrix(3, 0) = A(0, 0) * B(3, 0);
  matrix(3, 1) = A(0, 1) * B(3, 0);
  matrix(3, 2) = A(0, 2) * B(3, 0);
  matrix(3, 3) = A(0, 3) * B(3, 0);

  return matrix;
}

Math::Matrix<3, 4>    operator*(Math::Matrix<1, 4> const & A, Math::Matrix<3, 1> const & B)
{
  Math::Matrix<3, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);
  matrix(0, 2) = A(0, 2) * B(0, 0);
  matrix(0, 3) = A(0, 3) * B(0, 0);

  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(1, 1) = A(0, 1) * B(1, 0);
  matrix(1, 2) = A(0, 2) * B(1, 0);
  matrix(1, 3) = A(0, 3) * B(1, 0);

  matrix(2, 0) = A(0, 0) * B(2, 0);
  matrix(2, 1) = A(0, 1) * B(2, 0);
  matrix(2, 2) = A(0, 2) * B(2, 0);
  matrix(2, 3) = A(0, 3) * B(2, 0);

  return matrix;
}

Math::Matrix<2, 4>    operator*(Math::Matrix<1, 4> const & A, Math::Matrix<2, 1> const & B)
{
  Math::Matrix<2, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);
  matrix(0, 2) = A(0, 2) * B(0, 0);
  matrix(0, 3) = A(0, 3) * B(0, 0);

  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(1, 1) = A(0, 1) * B(1, 0);
  matrix(1, 2) = A(0, 2) * B(1, 0);
  matrix(1, 3) = A(0, 3) * B(1, 0);

  return matrix;
}

Math::Matrix<1, 4>    operator*(Math::Matrix<1, 4> const & A, Math::Matrix<1, 1> const & B)
{
  Math::Matrix<1, 4>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);
  matrix(0, 2) = A(0, 2) * B(0, 0);
  matrix(0, 3) = A(0, 3) * B(0, 0);

  return matrix;
}



Math::Matrix<4, 3>    operator*(Math::Matrix<1, 3> const & A, Math::Matrix<4, 1> const & B)
{
  Math::Matrix<4, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);
  matrix(0, 2) = A(0, 2) * B(0, 0);

  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(1, 1) = A(0, 1) * B(1, 0);
  matrix(1, 2) = A(0, 2) * B(1, 0);

  matrix(2, 0) = A(0, 0) * B(2, 0);
  matrix(2, 1) = A(0, 1) * B(2, 0);
  matrix(2, 2) = A(0, 2) * B(2, 0);

  matrix(3, 0) = A(0, 0) * B(3, 0);
  matrix(3, 1) = A(0, 1) * B(3, 0);
  matrix(3, 2) = A(0, 2) * B(3, 0);

  return matrix;
}

Math::Matrix<3, 3>    operator*(Math::Matrix<1, 3> const & A, Math::Matrix<3, 1> const & B)
{
  Math::Matrix<3, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);
  matrix(0, 2) = A(0, 2) * B(0, 0);

  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(1, 1) = A(0, 1) * B(1, 0);
  matrix(1, 2) = A(0, 2) * B(1, 0);

  matrix(2, 0) = A(0, 0) * B(2, 0);
  matrix(2, 1) = A(0, 1) * B(2, 0);
  matrix(2, 2) = A(0, 2) * B(2, 0);

  return matrix;
}

Math::Matrix<2, 3>    operator*(Math::Matrix<1, 3> const & A, Math::Matrix<2, 1> const & B)
{
  Math::Matrix<2, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);
  matrix(0, 2) = A(0, 2) * B(0, 0);

  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(1, 1) = A(0, 1) * B(1, 0);
  matrix(1, 2) = A(0, 2) * B(1, 0);

  return matrix;
}

Math::Matrix<1, 3>    operator*(Math::Matrix<1, 3> const & A, Math::Matrix<1, 1> const & B)
{
  Math::Matrix<1, 3>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);
  matrix(0, 2) = A(0, 2) * B(0, 0);

  return matrix;
}



Math::Matrix<4, 2>    operator*(Math::Matrix<1, 2> const & A, Math::Matrix<4, 1> const & B)
{
  Math::Matrix<4, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);

  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(1, 1) = A(0, 1) * B(1, 0);

  matrix(2, 0) = A(0, 0) * B(2, 0);
  matrix(2, 1) = A(0, 1) * B(2, 0);

  matrix(3, 0) = A(0, 0) * B(3, 0);
  matrix(3, 1) = A(0, 1) * B(3, 0);

  return matrix;
}

Math::Matrix<3, 2>    operator*(Math::Matrix<1, 2> const & A, Math::Matrix<3, 1> const & B)
{
  Math::Matrix<3, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);

  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(1, 1) = A(0, 1) * B(1, 0);

  matrix(2, 0) = A(0, 0) * B(2, 0);
  matrix(2, 1) = A(0, 1) * B(2, 0);

  return matrix;
}

Math::Matrix<2, 2>    operator*(Math::Matrix<1, 2> const & A, Math::Matrix<2, 1> const & B)
{
  Math::Matrix<2, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);

  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(1, 1) = A(0, 1) * B(1, 0);

  return matrix;
}

Math::Matrix<1, 2>    operator*(Math::Matrix<1, 2> const & A, Math::Matrix<1, 1> const & B)
{
  Math::Matrix<1, 2>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(0, 1) = A(0, 1) * B(0, 0);

  return matrix;
}



Math::Matrix<4, 1>    operator*(Math::Matrix<1, 1> const & A, Math::Matrix<4, 1> const & B)
{
  Math::Matrix<4, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(2, 0) = A(0, 0) * B(2, 0);
  matrix(3, 0) = A(0, 0) * B(3, 0);

  return matrix;
}

Math::Matrix<3, 1>    operator*(Math::Matrix<1, 1> const & A, Math::Matrix<3, 1> const & B)
{
  Math::Matrix<3, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(1, 0) = A(0, 0) * B(1, 0);
  matrix(2, 0) = A(0, 0) * B(2, 0);

  return matrix;
}

Math::Matrix<2, 1>    operator*(Math::Matrix<1, 1> const & A, Math::Matrix<2, 1> const & B)
{
  Math::Matrix<2, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);
  matrix(1, 0) = A(0, 0) * B(1, 0);

  return matrix;
}

Math::Matrix<1, 1>    operator*(Math::Matrix<1, 1> const & A, Math::Matrix<1, 1> const & B)
{
  Math::Matrix<1, 1>  matrix;

  matrix(0, 0) = A(0, 0) * B(0, 0);

  return matrix;
}
