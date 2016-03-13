#include "Intersection.hpp"

RT::Intersection::Intersection(RT::AbstractCsgTree const * node, RT::Ray const & norm, double dist)
  : node(node), normal(norm), distance(dist), material()
{}

RT::Intersection::~Intersection()
{}
