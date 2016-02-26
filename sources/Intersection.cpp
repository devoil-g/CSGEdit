#include "Intersection.hpp"

RT::Intersection::Intersection(RT::Ray const & norm, double dist)
  : node(nullptr), normal(norm), distance(dist), material()
{}

RT::Intersection::~Intersection()
{}
