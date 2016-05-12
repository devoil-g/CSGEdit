#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "Matrix.hpp"
#include "Vector.hpp"

namespace RT
{
  class Ray
  {
  private:
    Math::Vector<4>		_p, _d;	// Coordonnées de l'origine et de vecteur direction

  public:
    Ray() { _p(3) = 1.f; };
    ~Ray() {};

    Ray(Math::Vector<4> const & p, Math::Vector<4> const & d) : _p(p), _d(d) {};

    inline Math::Vector<4> &	p() { return _p; };		// Return point of origin matrix
    inline Math::Vector<4> &	d() { return _d; };		// Return direction vector matrix
    inline Math::Vector<4>	p() const { return _p; };	// Return point of origin matrix
    inline Math::Vector<4>	d() const { return _d; };	// Return direction vector matrix

    inline double			length() const							// Return ray length
    {
      return std::sqrt(_d.x() * _d.x() + _d.y() * _d.y() + _d.z() * _d.z());
    }

    inline RT::Ray			normalize() const						// Return a normalized ray
    {
      double	l = length();

      return RT::Ray(Math::Vector<4>(p().x(), p().y(), p().z(), (double)1.f), Math::Vector<4>(d().x() / l, d().y() / l, d().z() / l, (double)0.f));
    }

    static inline double		cos(Math::Vector<4> const & A, Math::Vector<4> const & B)	// Calculate cosinus between two rays
    {
#ifdef _DEBUG
      // Should not happen
      if ((A.x() * A.x() + A.y() * A.y() + A.z() * A.z()) * (B.x() * B.x() + B.y() * B.y() + B.z() * B.z()) == 0.f)
	throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
#endif

      return RT::Ray::scalaire(A, B) / std::sqrt((A.x() * A.x() + A.y() * A.y() + A.z() * A.z()) * (B.x() * B.x() + B.y() * B.y() + B.z() * B.z()));
    }

    static inline double		angle(Math::Vector<4> const & A, Math::Vector<4> const & B)	// Calculate angle (radian) between two rays
    {
      return std::acos(RT::Ray::cos(A, B));
    }

    static inline double		scalaire(Math::Vector<4> const & A, Math::Vector<4> const & B)	// Apply scalaire to rays
    {
      return A.x() * B.x() + A.y() * B.y() + A.z() * B.z();
    }

    static inline Math::Vector<4>	vectoriel(Math::Vector<4> const & A, Math::Vector<4> const & B)	// Apply vectoriel to rays
    {
      return Math::Vector<4>(A.y() * B.z() - A.z() * B.y(), A.z() * B.x() - A.x() * B.z(), A.x() * B.y() - A.y() * B.x(), (double)0.f);
    }
  };
};

inline RT::Ray	operator*(Math::Matrix<4, 4> const & matrix, RT::Ray const & ray)
{
  return RT::Ray(matrix * ray.p(), matrix * ray.d());
}

#endif
