#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "Matrix.hpp"

namespace Math
{
  class Ray
  {
  private:
    Math::Matrix<4, 1>		_p, _d;	// Coordonnées de l'origine et de vecteur direction

  public:
    Ray();
    ~Ray();

    static double		cos(Math::Ray const &, Math::Ray const &);		// Calculate cosinus between two rays
    static double		angle(Math::Ray const &, Math::Ray const &);		// Calculate angle (radian) between two rays
    static double		scalaire(Math::Ray const &, Math::Ray const &);		// Apply scalaire to rays
    static Math::Ray		vectoriel(Math::Ray const &, Math::Ray const &);	// Apply vectoriel to rays

    Math::Ray			normalize() const;					// Return a normalized ray

    inline Math::Matrix<4, 1> &	p() { return _p; };			// Return point of origin matrix
    inline Math::Matrix<4, 1> &	d() { return _d; };			// Return direction vector matrix
    inline Math::Matrix<4, 1>	p() const { return _p; };		// Return point of origin matrix
    inline Math::Matrix<4, 1>	d() const { return _d; };		// Return direction vector matrix

    inline double &		px() { return _p(0, 0); };		// Return X component of point of origin
    inline double &		py() { return _p(1, 0); };		// Return Y component of point of origin
    inline double &		pz() { return _p(2, 0); };		// Return Z component of point of origin
    inline double		px() const { return _p(0, 0); };	// Return X component of point of origin
    inline double		py() const { return _p(1, 0); };	// Return Y component of point of origin
    inline double		pz() const { return _p(2, 0); };	// Return Z component of point of origin

    inline double &		dx() { return _d(0, 0); };		// Return X component of direction vector
    inline double &		dy() { return _d(1, 0); };		// Return Y component of direction vector
    inline double &		dz() { return _d(2, 0); };		// Return Z component of direction vector
    inline double		dx() const { return _d(0, 0); };	// Return X component of direction vector
    inline double		dy() const { return _d(1, 0); };	// Return Y component of direction vector
    inline double		dz() const { return _d(2, 0); };	// Return Z component of direction vector
  };
};

Math::Ray operator*(Math::Matrix<4, 4> const &, Math::Ray const &);

#endif
