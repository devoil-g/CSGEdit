#include "Intersection.hpp"

RT::Intersection::Intersection(RT::Ray const & norm, double dist)
  : node(nullptr), normal(norm), distance(dist), material()
{}

RT::Intersection::~Intersection()
{}

bool	RT::Intersection::operator<(RT::Intersection const & inter) const
{
  // Compare intersections distances
  return this->distance < inter.distance;
}
