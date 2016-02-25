#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

#include "Exception.hpp"
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

    inline Math::Vector<vSize> &	operator=(Math::Matrix<vSize, 1> const & m)
    {
      Math::Matrix<vSize, 1>::operator=(m);
      return *this;
    }

    inline double &	operator()(unsigned int c) { return Math::Matrix<vSize, 1>::operator()(c, 0); }
    inline double	operator()(unsigned int c) const { return Math::Matrix<vSize, 1>::operator()(c, 0); }

    inline double &	x() { return (*this)(0); }
    inline double &	y() { return (*this)(1); }
    inline double &	z() { return (*this)(2); }

    inline double	x() const { return (*this)(0); }
    inline double	y() const { return (*this)(1); }
    inline double	z() const { return (*this)(2); }

    inline Math::Vector<vSize> &	operator*=(Math::Vector<vSize> const & v)
    {
      for (unsigned int i = 0; i < vSize; i++)
	(*this)(i) *= v(i);
      return *this;
    }

    inline Math::Vector<vSize>		operator*(Math::Vector<vSize> const & v) const
    {
      return Math::Vector<vSize>(*this) *= v;
    }

    inline Math::Vector<vSize> &	operator*=(double c)
    {
      Math::Matrix<vSize, 1>::operator*=(c);
      return *this;
    }

    inline Math::Vector<vSize>		operator*(double c) const
    {
      return Math::Vector<vSize>(*this) *= c;
    }
  };
};

#endif