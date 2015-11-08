#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <cstring>

#include "Math.hpp"

namespace Math
{

  template<unsigned int cCol, unsigned int cRow>
  class Matrix
  {
    double				_matrix[cCol][cRow];					// Matrix value container

  public:
    Matrix() : _matrix{0} {};
    ~Matrix() {};

    inline double &			operator()(unsigned int col, unsigned int row)		// Get matrix value
    {
#ifdef _DEBUG
      if (col >= cCol || row >= cRow)
	throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif
      return _matrix[col][row];
    };
    
    inline double			operator()(unsigned int col, unsigned int row) const	// Get matrix value
    {
#ifdef _DEBUG
      if (col >= cCol || row >= cRow)
	throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif
      return _matrix[col][row];
    };

    Math::Matrix<cCol, cRow> &		operator=(Math::Matrix<cCol, cRow> const & cpy)		// Copy matrix
    {
      std::memcpy(this->_matrix, cpy._matrix, sizeof(this->_matrix));
      return *this;
    };

    Math::Matrix<cCol, cRow>		inverse() const;					// Generate inverse matrix
    Math::Matrix<cCol, cRow>		transpose() const;					// Generate transpose matrix

    static Math::Matrix<cCol, cRow>	identite();						// Generate identity matrix
    static Math::Matrix<3, 3>		translation(double, double);				// Generate translation matrix
    static Math::Matrix<4, 4>		translation(double, double, double);			// Generate translation matrix
    static Math::Matrix<3, 3>		reflection(double, double);				// Generate mirror matrix
    static Math::Matrix<4, 4>		reflection(double, double, double);			// Generate mirror matrix
    static Math::Matrix<3, 3>		rotation(double);					// Generate rotation matrix
    static Math::Matrix<4, 4>		rotation(double, double, double);			// Generate rotation matrix
    static Math::Matrix<4, 4>		rotation(double, double, double, double);		// Generate rotation matrix along an axis
    static Math::Matrix<3, 3>		scale(double, double);					// Generate scaling matrix
    static Math::Matrix<4, 4>		scale(double, double, double);				// Generate scaling matrix
    static Math::Matrix<4, 4>		shear(double, double, double, double, double, double);	// Generate shearing matrix
  };
};

Math::Matrix<4, 4>  operator*(Math::Matrix<4, 4> const &, Math::Matrix<4, 4> const &);
Math::Matrix<3, 4>  operator*(Math::Matrix<4, 4> const &, Math::Matrix<3, 4> const &);
Math::Matrix<2, 4>  operator*(Math::Matrix<4, 4> const &, Math::Matrix<2, 4> const &);
Math::Matrix<1, 4>  operator*(Math::Matrix<4, 4> const &, Math::Matrix<1, 4> const &);

Math::Matrix<4, 3>  operator*(Math::Matrix<4, 3> const &, Math::Matrix<4, 4> const &);
Math::Matrix<3, 3>  operator*(Math::Matrix<4, 3> const &, Math::Matrix<3, 4> const &);
Math::Matrix<2, 3>  operator*(Math::Matrix<4, 3> const &, Math::Matrix<2, 4> const &);
Math::Matrix<1, 3>  operator*(Math::Matrix<4, 3> const &, Math::Matrix<1, 4> const &);

Math::Matrix<4, 2>  operator*(Math::Matrix<4, 2> const &, Math::Matrix<4, 4> const &);
Math::Matrix<3, 2>  operator*(Math::Matrix<4, 2> const &, Math::Matrix<3, 4> const &);
Math::Matrix<2, 2>  operator*(Math::Matrix<4, 2> const &, Math::Matrix<2, 4> const &);
Math::Matrix<1, 2>  operator*(Math::Matrix<4, 2> const &, Math::Matrix<1, 4> const &);

Math::Matrix<4, 1>  operator*(Math::Matrix<4, 1> const &, Math::Matrix<4, 4> const &);
Math::Matrix<3, 1>  operator*(Math::Matrix<4, 1> const &, Math::Matrix<3, 4> const &);
Math::Matrix<2, 1>  operator*(Math::Matrix<4, 1> const &, Math::Matrix<2, 4> const &);
Math::Matrix<1, 1>  operator*(Math::Matrix<4, 1> const &, Math::Matrix<1, 4> const &);

// -----------------------------------------------------------------------------------

Math::Matrix<4, 4>  operator*(Math::Matrix<3, 4> const &, Math::Matrix<4, 3> const &);
Math::Matrix<3, 4>  operator*(Math::Matrix<3, 4> const &, Math::Matrix<3, 3> const &);
Math::Matrix<2, 4>  operator*(Math::Matrix<3, 4> const &, Math::Matrix<2, 3> const &);
Math::Matrix<1, 4>  operator*(Math::Matrix<3, 4> const &, Math::Matrix<1, 3> const &);

Math::Matrix<4, 3>  operator*(Math::Matrix<3, 3> const &, Math::Matrix<4, 3> const &);
Math::Matrix<3, 3>  operator*(Math::Matrix<3, 3> const &, Math::Matrix<3, 3> const &);
Math::Matrix<2, 3>  operator*(Math::Matrix<3, 3> const &, Math::Matrix<2, 3> const &);
Math::Matrix<1, 3>  operator*(Math::Matrix<3, 3> const &, Math::Matrix<1, 3> const &);

Math::Matrix<4, 2>  operator*(Math::Matrix<3, 2> const &, Math::Matrix<4, 3> const &);
Math::Matrix<3, 2>  operator*(Math::Matrix<3, 2> const &, Math::Matrix<3, 3> const &);
Math::Matrix<2, 2>  operator*(Math::Matrix<3, 2> const &, Math::Matrix<2, 3> const &);
Math::Matrix<1, 2>  operator*(Math::Matrix<3, 2> const &, Math::Matrix<1, 3> const &);

Math::Matrix<4, 1>  operator*(Math::Matrix<3, 1> const &, Math::Matrix<4, 3> const &);
Math::Matrix<3, 1>  operator*(Math::Matrix<3, 1> const &, Math::Matrix<3, 3> const &);
Math::Matrix<2, 1>  operator*(Math::Matrix<3, 1> const &, Math::Matrix<2, 3> const &);
Math::Matrix<1, 1>  operator*(Math::Matrix<3, 1> const &, Math::Matrix<1, 3> const &);

// -----------------------------------------------------------------------------------

Math::Matrix<4, 4>  operator*(Math::Matrix<2, 4> const &, Math::Matrix<4, 2> const &);
Math::Matrix<3, 4>  operator*(Math::Matrix<2, 4> const &, Math::Matrix<3, 2> const &);
Math::Matrix<2, 4>  operator*(Math::Matrix<2, 4> const &, Math::Matrix<2, 2> const &);
Math::Matrix<1, 4>  operator*(Math::Matrix<2, 4> const &, Math::Matrix<1, 2> const &);

Math::Matrix<4, 3>  operator*(Math::Matrix<2, 3> const &, Math::Matrix<4, 2> const &);
Math::Matrix<3, 3>  operator*(Math::Matrix<2, 3> const &, Math::Matrix<3, 2> const &);
Math::Matrix<2, 3>  operator*(Math::Matrix<2, 3> const &, Math::Matrix<2, 2> const &);
Math::Matrix<1, 3>  operator*(Math::Matrix<2, 3> const &, Math::Matrix<1, 2> const &);

Math::Matrix<4, 2>  operator*(Math::Matrix<2, 2> const &, Math::Matrix<4, 2> const &);
Math::Matrix<3, 2>  operator*(Math::Matrix<2, 2> const &, Math::Matrix<3, 2> const &);
Math::Matrix<2, 2>  operator*(Math::Matrix<2, 2> const &, Math::Matrix<2, 2> const &);
Math::Matrix<1, 2>  operator*(Math::Matrix<2, 2> const &, Math::Matrix<1, 2> const &);

Math::Matrix<4, 1>  operator*(Math::Matrix<2, 1> const &, Math::Matrix<4, 2> const &);
Math::Matrix<3, 1>  operator*(Math::Matrix<2, 1> const &, Math::Matrix<3, 2> const &);
Math::Matrix<2, 1>  operator*(Math::Matrix<2, 1> const &, Math::Matrix<2, 2> const &);
Math::Matrix<1, 1>  operator*(Math::Matrix<2, 1> const &, Math::Matrix<1, 2> const &);

// -----------------------------------------------------------------------------------

Math::Matrix<4, 4>  operator*(Math::Matrix<1, 4> const &, Math::Matrix<4, 1> const &);
Math::Matrix<3, 4>  operator*(Math::Matrix<1, 4> const &, Math::Matrix<3, 1> const &);
Math::Matrix<2, 4>  operator*(Math::Matrix<1, 4> const &, Math::Matrix<2, 1> const &);
Math::Matrix<1, 4>  operator*(Math::Matrix<1, 4> const &, Math::Matrix<1, 1> const &);

Math::Matrix<4, 3>  operator*(Math::Matrix<1, 3> const &, Math::Matrix<4, 1> const &);
Math::Matrix<3, 3>  operator*(Math::Matrix<1, 3> const &, Math::Matrix<3, 1> const &);
Math::Matrix<2, 3>  operator*(Math::Matrix<1, 3> const &, Math::Matrix<2, 1> const &);
Math::Matrix<1, 3>  operator*(Math::Matrix<1, 3> const &, Math::Matrix<1, 1> const &);

Math::Matrix<4, 2>  operator*(Math::Matrix<1, 2> const &, Math::Matrix<4, 1> const &);
Math::Matrix<3, 2>  operator*(Math::Matrix<1, 2> const &, Math::Matrix<3, 1> const &);
Math::Matrix<2, 2>  operator*(Math::Matrix<1, 2> const &, Math::Matrix<2, 1> const &);
Math::Matrix<1, 2>  operator*(Math::Matrix<1, 2> const &, Math::Matrix<1, 1> const &);

Math::Matrix<4, 1>  operator*(Math::Matrix<1, 1> const &, Math::Matrix<4, 1> const &);
Math::Matrix<3, 1>  operator*(Math::Matrix<1, 1> const &, Math::Matrix<3, 1> const &);
Math::Matrix<2, 1>  operator*(Math::Matrix<1, 1> const &, Math::Matrix<2, 1> const &);
Math::Matrix<1, 1>  operator*(Math::Matrix<1, 1> const &, Math::Matrix<1, 1> const &);

// -----------------------------------------------------------------------------------

// Old universal matrix multiplier (bad performances, loops!)
/*
Math::Matrix	Math::Matrix::operator*(Math::Matrix const & B) const
{
  Math::Matrix const &	A = *this;
  Math::Matrix		matrix(B.col(), A.row());

#ifdef _DEBUG
  if (A.col() != B.row())
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

  for (unsigned int col = 0; col < B.col(); col++)
    for (unsigned int row = 0; row < A.row(); row++)
      for (unsigned int x = 0; x < A.col(); x++)
	matrix(col, row) += A(x, row) * B(col, x);

  return matrix;
}
*/

#endif