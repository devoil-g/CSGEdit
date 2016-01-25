#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <cstring>
#include <sstream>
#include <string>

#include "Exception.hpp"
#include "Math.hpp"

namespace Math
{
  unsigned int const	MaxMatrixSize(4);

  template<unsigned int cRow, unsigned int cCol>
  class Matrix
  {
  private:
    double				_matrix[cRow][cCol];					// Matrix value container

  public:
    Matrix()
      : _matrix{ 0 }
    {
      // Compilation time error if invalid matrix
      static_assert(cRow > 0 && cRow <= Math::MaxMatrixSize && cCol > 0 && cCol <= Math::MaxMatrixSize, "Invalid matrix size.");
    }

    Matrix(Math::Matrix<cRow, cCol> const & cpy)
    {
      std::memcpy(this->_matrix, cpy._matrix, sizeof(this->_matrix));
    }

    ~Matrix() {};

    Math::Matrix<cRow, cCol> &		operator=(Math::Matrix<cRow, cCol> const & cpy)		// Copy matrix
    {
      std::memcpy(this->_matrix, cpy._matrix, sizeof(this->_matrix));
      return *this;
    }
    
    inline double &			operator()(unsigned int row, unsigned int col)		// Get matrix value
    {
#ifdef _DEBUG
      if (row >= cRow || col >= cCol)
	throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif
      return _matrix[row][col];
    };
    
    inline double			operator()(unsigned int row, unsigned int col) const	// Get matrix value
    {
#ifdef _DEBUG
      if (row >= cRow || col >= cCol)
	throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif
      return _matrix[row][col];
    };
    
    template<unsigned int dRow, unsigned int dCol>
    Math::Matrix<cRow, dCol>		operator*(Math::Matrix<dRow, dCol> const & m) const	// Matrix multiplication
    {
      Math::Matrix<cRow, dCol>		matrix;

      // Compilation time error if invalid matrix
      static_assert(dRow == cCol, "Invalid matrix multiplication.");

      for (unsigned int row = 0; row < cRow; row++)
	for (unsigned int col = 0; col < dCol; col++)
	  for (unsigned int x = 0; x < cCol; x++)
	    matrix(row, col) += (*this)(row, x) * m(x, col);

      return matrix;
    }

    Math::Matrix<cRow, cCol>		transpose() const					// Generate transpose matrix
    {
      Math::Matrix<cRow, cCol>		matrix;

      // Compilation time error if invalid matrix
      static_assert(cRow == cCol, "Invalid matrix transpose.");

      for (unsigned int row = 0; row < cRow; row++)
	for (unsigned int col = 0; col < cCol; col++)
	  matrix(row, col) = (*this)(col, row);

      return matrix;
    }

    static Math::Matrix<cRow, cCol>	identite()						// Generate identity matrix
    {
      Math::Matrix<cRow, cCol>  matrix;

      // Compilation time error if invalid matrix
      static_assert(cRow == cCol, "Invalid matrix.");

      for (unsigned int i = 0; i < cRow; i++)
	matrix(i, i) = 1.f;

      return matrix;
    }

    std::string				dump() const
    {
      std::stringstream			stream;

      stream << "[";
      for (unsigned int row = 0; row < cRow; row++)
      {
	if (row != 0)
	  stream << ", ";
	stream << "[";

	for (unsigned int col = 0; col < cCol; col++)
	{
	  if (col != 0)
	    stream << ", ";
	  stream << _matrix[row][col];
	}
	
	stream << "]";
      }
      stream << "]";

      return stream.str();
    }

    // Methods specialized in Matrix.cpp
    Math::Matrix<cRow, cCol>		inverse() const;					// Generate inverse matrix
    static Math::Matrix<cRow, cCol>	translation(double, double);				// Generate translation matrix
    static Math::Matrix<cRow, cCol>	translation(double, double, double);			// Generate translation matrix
    static Math::Matrix<cRow, cCol>	reflection(double, double);				// Generate mirror matrix
    static Math::Matrix<cRow, cCol>	reflection(double, double, double);			// Generate mirror matrix
    static Math::Matrix<cRow, cCol>	rotation(double);					// Generate rotation matrix
    static Math::Matrix<cRow, cCol>	rotation(double, double, double);			// Generate rotation matrix
    static Math::Matrix<cRow, cCol>	rotation(double, double, double, double);		// Generate rotation matrix
    static Math::Matrix<cRow, cCol>	scale(double);						// Generate scaling matrix
    static Math::Matrix<cRow, cCol>	scale(double, double);					// Generate scaling matrix
    static Math::Matrix<cRow, cCol>	scale(double, double, double);				// Generate scaling matrix
    static Math::Matrix<cRow, cCol>	shear(double, double, double, double, double, double);	// Generate shearing matrix
  };
};

#endif