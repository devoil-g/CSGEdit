#ifndef _MATRIX_4X4_HPP_
#define _MATRIX_4X4_HPP_

#include <immintrin.h>

#ifdef _DEBUG
#include <stdexcept>
#endif

#include "Math.hpp"
#include "Vector4.hpp"

namespace Math
{
  class Matrix4x4
  {
  protected:
    Math::Vector4			_matrix[4];						// Matrix value container

  public:
    inline Matrix4x4() {}
    inline Matrix4x4(Math::Vector4 v) : _matrix{ v, v, v, v } {}
    inline Matrix4x4(Math::Vector4 v0, Math::Vector4 v1, Math::Vector4 v2, Math::Vector4 v3) : _matrix{ v0, v1, v2, v3 } {}
    inline ~Matrix4x4() {};

    inline float &			operator()(unsigned int row, unsigned int col)		// Get/set matrix value
    {
#ifdef _DEBUG
      if (row >= 4 || col >= 4)
	throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
#endif

      return _matrix[row][col];
    };

    inline float			operator()(unsigned int row, unsigned int col) const	// Get matrix value
    {
#ifdef _DEBUG
      if (row >= 4 || col >= 4)
	throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
#endif

      return _matrix[row][col];
    };

    Math::Vector4			operator*(Math::Vector4 const & v)			// Vector multiplication
    {
      Math::Vector4			result;

      for (int row = 0; row < 4; row++)
      {
	__m128				line = _mm_mul_ps(_matrix[row]._vector.sse, v._vector.sse);
	
	line = _mm_hadd_ps(line, line);
	line = _mm_hadd_ps(line, line);
	
	_mm_store_ss(&result[row], line);
      }

      return result;
    }

    Math::Matrix4x4			operator*(Math::Matrix4x4 const & m)			// Matrix multiplication
    {
      Math::Matrix4x4			result;

      for (unsigned int i = 0; i < 4; i++)
	for (unsigned int j = 0; j < 4; j++)
	  result._matrix[i] += Math::Vector4(_matrix[j]._vector.sse) * Math::Vector4(m._matrix[i]._vector.value[j]);

      return result;
    }

    Math::Matrix4x4			transpose() const					// Generate transpose matrix
    {
      __m128				tmp0 = _mm_shuffle_ps(_matrix[0]._vector.sse, _matrix[1]._vector.sse, 0x44);
      __m128				tmp2 = _mm_shuffle_ps(_matrix[0]._vector.sse, _matrix[1]._vector.sse, 0xEE);
      __m128				tmp1 = _mm_shuffle_ps(_matrix[2]._vector.sse, _matrix[3]._vector.sse, 0x44);
      __m128				tmp3 = _mm_shuffle_ps(_matrix[2]._vector.sse, _matrix[3]._vector.sse, 0xEE);

      return Math::Matrix4x4(_mm_shuffle_ps(tmp0, tmp1, 0x88), _mm_shuffle_ps(tmp0, tmp1, 0xDD), _mm_shuffle_ps(tmp2, tmp3, 0x88), _mm_shuffle_ps(tmp2, tmp3, 0xDD));
    }

    static inline Math::Matrix4x4	identite()						// Generate identity matrix
    {
      return Math::Matrix4x4(Math::Vector4(1.f, 0.f, 0.f, 0.f), Math::Vector4(0.f, 1.f, 0.f, 0.f), Math::Vector4(0.f, 0.f, 1.f, 0.f), Math::Vector4(0.f, 0.f, 0.f, 1.f));
    }

    static inline Math::Matrix4x4	translation(float x, float y, float z)			// Generate translation matrix
    {
      return Math::Matrix4x4(Math::Vector4(1.f, 0.f, 0.f, x), Math::Vector4(0.f, 1.f, 0.f, y), Math::Vector4(0.f, 0.f, 1.f, z), Math::Vector4(0.f, 0.f, 0.f, 1.f));
    }

    static inline Math::Matrix4x4	scale(float x, float y, float z)			// Generate translation matrix
    {
      return Math::Matrix4x4(Math::Vector4(x, 0.f, 0.f, 0.f), Math::Vector4(0.f, y, 0.f, 0.f), Math::Vector4(0.f, 0.f, z, 0.f), Math::Vector4(0.f, 0.f, 0.f, 1.f));
    }

    static inline Math::Matrix4x4	scale(float s)						// Generate translation matrix
    {
      return Math::Matrix4x4::scale(s, s, s);
    }

    // Methods specialized in Matrix4x4.cpp
    Math::Matrix4x4			inverse() const;					// Generate inverse matrix
    static Math::Matrix4x4		reflection(float, float, float);			// Generate mirror matrix
    static Math::Matrix4x4		rotation(float, float, float);				// Generate rotation matrix
    static Math::Matrix4x4		rotation(float, float, float, float);			// Generate rotation matrix
    static Math::Matrix4x4		shear(float, float, float, float, float, float);	// Generate shearing matrix
  };
};

#endif
