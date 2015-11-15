#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <cstring>

#include "Math.hpp"

namespace Math
{
  unsigned int const	MaxMatrixSize(4);

  template<unsigned int cCol, unsigned int cRow>
  class Matrix
  {
  private:
    double				_matrix[cCol][cRow];					// Matrix value container

  public:
    Matrix()
      : _matrix{ 0 }
    {
      // Compilation time error if invalid matrix
      static_assert(cCol > 0 && cCol <= Math::MaxMatrixSize && cRow > 0 && cRow <= Math::MaxMatrixSize, "Invalid matrix size.");
    }

    Matrix(Math::Matrix<cCol, cRow> const & cpy)
    {
      std::memcpy(this->_matrix, cpy._matrix, sizeof(this->_matrix));
    }

    ~Matrix() {};

    Math::Matrix<cCol, cRow> &		operator=(Math::Matrix<cCol, cRow> const & cpy)		// Copy matrix
    {
      std::memcpy(this->_matrix, cpy._matrix, sizeof(this->_matrix));
      return *this;
    }
    
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
    
    template<unsigned int dCol, unsigned int dRow>
    Math::Matrix<dCol, cRow>		operator*(Math::Matrix<dCol, dRow> const & m) const	// Matrix multiplication
    {
      Math::Matrix<dCol, cRow>		matrix;

      // Compilation time error if invalid matrix
      static_assert(cCol == dRow, "Invalid matrix multiplication.");

      for (unsigned int col = 0; col < dCol; col++)
	for (unsigned int row = 0; row < cRow; row++)
	  for (unsigned int x = 0; x < cCol; x++)
	    matrix(col, row) += (*this)(x, row) * m(col, x);

      return matrix;
    }

    Math::Matrix<cCol, cRow>		transpose() const					// Generate transpose matrix
    {
      Math::Matrix<cCol, cRow>		matrix;

      // Compilation time error if invalid matrix
      static_assert(cCol == cRow, "Invalid matrix transpose.");

      for (unsigned int col = 0; col < cCol; col++)
	for (unsigned int row = 0; row < cRow; row++)
	  matrix(row, col) = (*this)(col, row);

      return matrix;
    }

    static Math::Matrix<cCol, cRow>	identite()						// Generate identity matrix
    {
      Math::Matrix<cCol, cRow>  matrix;

      // Compilation time error if invalid matrix
      static_assert(cCol == cRow, "Invalid matrix.");

      for (unsigned int i = 0; i < cCol; i++)
	matrix(i, i) = 1.f;

      return matrix;
    }

    // Methods specialized in Matrix.cpp
    Math::Matrix<cCol, cRow>		inverse() const;					// Generate inverse matrix
    static Math::Matrix<cCol, cRow>	translation(double, double);				// Generate translation matrix
    static Math::Matrix<cCol, cRow>	translation(double, double, double);			// Generate translation matrix
    static Math::Matrix<cCol, cRow>	reflection(double, double);				// Generate mirror matrix
    static Math::Matrix<cCol, cRow>	reflection(double, double, double);			// Generate mirror matrix
    static Math::Matrix<cCol, cRow>	rotation(double);					// Generate rotation matrix
    static Math::Matrix<cCol, cRow>	rotation(double, double, double);			// Generate rotation matrix
    static Math::Matrix<cCol, cRow>	rotation(double, double, double, double);		// Generate rotation matrix
    static Math::Matrix<cCol, cRow>	scale(double);						// Generate scaling matrix
    static Math::Matrix<cCol, cRow>	scale(double, double);					// Generate scaling matrix
    static Math::Matrix<cCol, cRow>	scale(double, double, double);				// Generate scaling matrix
    static Math::Matrix<cCol, cRow>	shear(double, double, double, double, double, double);	// Generate shearing matrix
  };
};

#endif