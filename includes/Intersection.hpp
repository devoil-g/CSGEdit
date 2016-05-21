#ifndef _INTERSECTION_HPP_
#define _INTERSECTION_HPP_

#include "Material.hpp"
#include "Ray.hpp"

namespace RT
{
  class Intersection
  {
  public:
    Intersection(RT::Ray const & n, double d) : normal(n), distance(d), material() {};
    ~Intersection() {};

    RT::Ray		normal;		// Normal with intersected object
    double		distance;	// Distance of intersection
    RT::Material	material;	// Material to apply

    inline bool		operator<(RT::Intersection const & i) const	// Compare distances to sort intersections list
    {
      return distance < i.distance;
    }
  };
};

#endif
