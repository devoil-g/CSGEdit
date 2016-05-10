#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <cstring>
#include <sstream>
#include <string>

#ifdef _DEBUG
#include <stdexcept>
#endif

#include "Math.hpp"

namespace Math
{
  template<unsigned int cRow, unsigned int cCol>
  class Matrix
  {
  protected:
    double				_matrix[cRow][cCol];					// Matrix value container

  public:
    Matrix()
      : _matrix{ 0 }
    {
      // Compilation time error if invalid matrix
      static_assert(cRow > 0 && cCol > 0, "Invalid matrix size.");
    }
    
    ~Matrix() {};
    
    bool				operator==(Math::Matrix<cRow, cCol> const & m)		// Matrix comparison
    {
      for (unsigned int row = 0; row < cRow; row++)
	for (unsigned int col = 0; col < cCol; col++)
	  if ((*this)(row, col) != m(row, col))
	    return false;
      return true;
    }

    bool				operator!=(Math::Matrix<cRow, cCol> const & m)		// Matrix comparison
    {
      return !(*this == m);
    }

    inline double &			operator()(unsigned int row, unsigned int col)		// Get matrix value
    {
#ifdef _DEBUG
      if (row >= cRow || col >= cCol)
	throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
#endif
      return _matrix[row][col];
    };

    inline double			operator()(unsigned int row, unsigned int col) const	// Get matrix value
    {
#ifdef _DEBUG
      if (row >= cRow || col >= cCol)
	throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
#endif
      return _matrix[row][col];
    };

    Math::Matrix<cRow, cCol> &		operator+=(Math::Matrix<cRow, cCol> const & m)		// Matrix addition
    {
      for (unsigned int row = 0; row < cRow; row++)
	for (unsigned int col = 0; col < cCol; col++)
	  (*this)(row, col) += m(row, col);

      return *this;
    }

    Math::Matrix<cRow, cCol>		operator+(Math::Matrix<cRow, cCol> const & m) const	// Matrix addition
    {
      return Math::Matrix<cRow, cCol>(*this) += m;
    }

    Math::Matrix<cRow, cCol> &		operator-=(Math::Matrix<cRow, cCol> const & m)		// Matrix subtraction
    {
      for (unsigned int row = 0; row < cRow; row++)
	for (unsigned int col = 0; col < cCol; col++)
	  (*this)(row, col) -= m(row, col);

      return *this;
    }

    Math::Matrix<cRow, cCol>		operator-(Math::Matrix<cRow, cCol> const & m) const	// Matrix subtraction
    {
      return Math::Matrix<cRow, cCol>(*this) -= m;
    }

    Math::Matrix<cRow, cCol> &		operator/=(double v)					// Matrix division
    {
#ifdef _DEBUG
      if (v == 0.f)
	throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
#endif
      for (unsigned int row = 0; row < cRow; row++)
	for (unsigned int col = 0; col < cCol; col++)
	  (*this)(row, col) /= v;

      return *this;
    }

    Math::Matrix<cRow, cCol>		operator/(double v) const				// Matrix division
    {
      return Math::Matrix<cRow, cCol>(*this) /= v;
    }

    Math::Matrix<cRow, cCol> &		operator*=(double v)					// Matrix matrix multiplication
    {
      for (unsigned int row = 0; row < cRow; row++)
	for (unsigned int col = 0; col < cCol; col++)
	  (*this)(row, col) *= v;

      return *this;
    }

    Math::Matrix<cRow, cCol>		operator*(double v) const				// Matrix multiplication
    {
      return Math::Matrix<cRow, cCol>(*this) *= v;
    }

    template<unsigned int dRow, unsigned int dCol>
    Math::Matrix<cRow, dCol> &		operator*=(Math::Matrix<dRow, dCol> const & m)		// Matrix multiplication
    {
      *this = *this * m;
      return *this;
    }

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
      static_assert(cRow == cCol, "Invalid matrix identite.");

      for (unsigned int i = 0; i < cRow; i++)
	matrix(i, i) = 1.f;

      return matrix;
    }

    template<typename ... Doubles>
    static Math::Matrix<cRow, cCol>	translation(Doubles... args)				// Generate translation matrix
    {
      Math::Matrix<cRow, cCol>	matrix = Math::Matrix<cRow, cCol>::identite();
      double			transformation[]{ args... };

      static_assert(cRow == cCol && cRow > 1, "Invalid translation matrix.");
      static_assert(sizeof(transformation) / sizeof(double) == cRow - 1, "Invalid translation matrix parameters.");

      for (unsigned int i = 0; i < cRow - 1; i++)
	matrix(i, cCol - 1) = transformation[i];

      return matrix;
    }

    template<typename ... Doubles>
    static Math::Matrix<cRow, cCol>	scale(Doubles... args)					// Generate scaling matrix
    {
      Math::Matrix<cRow, cCol>	matrix = Math::Matrix<cRow, cCol>::identite();
      double			transformation[]{ args... };

      static_assert(cRow == cCol && cRow > 1, "Invalid scale matrix.");
      static_assert((sizeof(transformation) / sizeof(double) == cRow - 1) || (sizeof(transformation) / sizeof(double) == 1), "Invalid scale matrix parameters.");

      if (sizeof(transformation) / sizeof(double) == cRow - 1)
	for (unsigned int i = 0; i < cRow - 1; i++)
	  matrix(i, i) = transformation[i];
      else
	for (unsigned int i = 0; i < cRow - 1; i++)
	  matrix(i, i) = transformation[0];

      return matrix;
    }

    // Methods specialized in Matrix.cpp
    Math::Matrix<cRow, cCol>		inverse() const;					// Generate inverse matrix
    static Math::Matrix<cRow, cCol>	reflection(double, double);				// Generate mirror matrix
    static Math::Matrix<cRow, cCol>	reflection(double, double, double);			// Generate mirror matrix
    static Math::Matrix<cRow, cCol>	rotation(double);					// Generate rotation matrix
    static Math::Matrix<cRow, cCol>	rotation(double, double, double);			// Generate rotation matrix
    static Math::Matrix<cRow, cCol>	rotation(double, double, double, double);		// Generate rotation matrix
    static Math::Matrix<cRow, cCol>	shear(double, double, double, double, double, double);	// Generate shearing matrix
  };
};

template<unsigned int cRow, unsigned int cCol>
Math::Matrix<cRow, cCol>	operator*(double v, Math::Matrix<cRow, cCol> const & m) { return m * v; }

#endif
