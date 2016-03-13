#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include "Math.hpp"
#include "Matrix.hpp"

namespace Math
{
  template<unsigned int vSize>
  class Vector : public Math::Matrix<vSize, 1>
  {
  public:
    Vector() {};
    Vector(Math::Matrix<vSize, 1> const & m) : Math::Matrix<vSize, 1>(m) {};
    ~Vector() {};

    template<typename ... Doubles>
    Vector(Doubles... args)
    {
      double	vec[]{ args... };

      static_assert(sizeof(vec) / sizeof(double) == vSize, "Invalid vector parameters.");

      for (unsigned int i = 0; i < vSize; i++)
	(*this)(i) = vec[i];
    };

    inline double &	operator()(unsigned int c) { return Math::Matrix<vSize, 1>::operator()(c, 0); }		// Get nth component of vector
    inline double	operator()(unsigned int c) const { return Math::Matrix<vSize, 1>::operator()(c, 0); }	// Get nth component of vector

    inline double &	x() { return (*this)(0); }		// Get first component of vector
    inline double &	y() { return (*this)(1); }		// Get second component of vector
    inline double &	z() { return (*this)(2); }		// Get third component of vector

    inline double	x() const { return (*this)(0); }	// Get first component of vector
    inline double	y() const { return (*this)(1); }	// Get second component of vector
    inline double	z() const { return (*this)(2); }	// Get third component of vector

    inline Math::Vector<vSize> &	operator=(Math::Matrix<vSize, 1> const & m)	// Vector sssignment operator
    {
      Math::Matrix<vSize, 1>::operator=(m);
      return *this;
    }

    inline Math::Vector<vSize> &	operator*=(Math::Vector<vSize> const & v)	// Vector multiplication
    {
      for (unsigned int i = 0; i < vSize; i++)
	(*this)(i) *= v(i);
      return *this;
    }

    inline Math::Vector<vSize>		operator*(Math::Vector<vSize> const & v) const	// Vector multiplication
    {
      return Math::Vector<vSize>(*this) *= v;
    }

    inline Math::Vector<vSize> &	operator*=(double c)				// Vector multiplication
    {
      Math::Matrix<vSize, 1>::operator*=(c);
      return *this;
    }

    inline Math::Vector<vSize>		operator*(double c) const			// Vector multiplication
    {
      return Math::Vector<vSize>(*this) *= c;
    }
  };
};

#endif
