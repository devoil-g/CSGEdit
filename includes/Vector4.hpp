#ifndef _VECTOR_4_HPP_
#define _VECTOR_4_HPP_

#include <immintrin.h>

namespace Math
{
  class Vector4
  {
  private:
    __declspec(align(16)) union Vector
    {
      __m128			sse;
      float			value[4];

      inline Vector() : Vector(0.f) {}
      inline Vector(__m128 v) : sse(v) {}
      inline Vector(float v) : sse(_mm_set1_ps(v)) {}
      inline Vector(float const * v) : sse(_mm_load_ps(v)) {}
      inline Vector(float x, float y, float z, float w) : sse(_mm_set_ps(w, z, y, x)) {}
      inline ~Vector() {}
    };

  public:
    Math::Vector4::Vector	_vector;	// Vector SEE container

    inline Vector4() {}
    inline Vector4(__m128 v) : _vector(v) {}
    inline Vector4(float v) : _vector(v) {}
    inline Vector4(float const * v) : _vector(v) {}
    inline Vector4(float x, float y, float z, float w) : _vector(w, z, y, x) {}
    inline ~Vector4() {}

    // Const operator
    inline Math::Vector4	operator*(Math::Vector4 const & v) const { return _mm_mul_ps(_vector.sse, v._vector.sse); }
    inline Math::Vector4	operator+(Math::Vector4 const & v) const { return _mm_add_ps(_vector.sse, v._vector.sse); }
    inline Math::Vector4	operator-(Math::Vector4 const & v) const { return _mm_sub_ps(_vector.sse, v._vector.sse); }
    inline Math::Vector4	operator/(Math::Vector4 const & v) const { return _mm_div_ps(_vector.sse, v._vector.sse); }

    // Const getter / setter
    inline float		operator[](unsigned int n) const { return _vector.value[n]; }
    inline float		x() const { return (*this)[0]; }
    inline float		y() const { return (*this)[1]; }
    inline float		z() const { return (*this)[2]; }
    inline float		w() const { return (*this)[3]; }

    // Assignement operator
    inline Math::Vector4 &	operator*=(Math::Vector4 const & v) { return *this = *this * v; }
    inline Math::Vector4 &	operator+=(Math::Vector4 const & v) { return *this = *this + v; }
    inline Math::Vector4 &	operator-=(Math::Vector4 const & v) { return *this = *this - v; }
    inline Math::Vector4 &	operator/=(Math::Vector4 const & v) { return *this = *this / v; }

    // Getter / setter
    inline float &		operator[](unsigned int n) { return _vector.value[n]; }
    inline float &		x() { return (*this)[0]; }
    inline float &		y() { return (*this)[1]; }
    inline float &		z() { return (*this)[2]; }
    inline float &		w() { return (*this)[3]; }
  };
};

#endif
