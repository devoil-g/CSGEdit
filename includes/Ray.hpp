#ifndef _RAY_HPP_
#define _RAY_HPP_

#include "Matrix.hpp"
#include "Vector.hpp"

namespace RT
{
  class Ray
  {
  private:
    Math::Vector<4>		_p, _d;						// Coordonnées de l'origine et de vecteur direction

  public:
    Ray() { _p(3) = 1.f; }
    ~Ray() {};

    inline Math::Vector<4> &	p() { return _p; };				// Return point of origin matrix
    inline Math::Vector<4> &	d() { return _d; };				// Return direction vector matrix
    inline Math::Vector<4>	p() const { return _p; };			// Return point of origin matrix
    inline Math::Vector<4>	d() const { return _d; };			// Return direction vector matrix

    RT::Ray			normalize() const;				// Return a normalized ray

    static double		cos(RT::Ray const &, RT::Ray const &);		// Calculate cosinus between two rays
    static double		angle(RT::Ray const &, RT::Ray const &);	// Calculate angle (radian) between two rays
    static double		scalaire(RT::Ray const &, RT::Ray const &);	// Apply scalaire to rays
    static RT::Ray		vectoriel(RT::Ray const &, RT::Ray const &);	// Apply vectoriel to rays
  };
};

RT::Ray operator*(Math::Matrix<4, 4> const &, RT::Ray const &);

#endif
