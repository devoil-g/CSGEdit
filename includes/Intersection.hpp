#ifndef _INTERSECTION_HPP_
#define _INTERSECTION_HPP_

#include "Material.hpp"
#include "Ray.hpp"

namespace RT
{
  class AbstractTree;

  class Intersection
  {
  public:
    Intersection(RT::Ray const &, double);
    ~Intersection();

    RT::AbstractTree const *	node;			// Sub-tree containing intersection object
    RT::Ray			normal;			// Normal with intersected object
    double			distance;		// Distance of intersection
    RT::Material		material;		// Material to apply

    bool  operator<(RT::Intersection const &) const;	// Compare distances to sort intersections list
  };
};

#endif
