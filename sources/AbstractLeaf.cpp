#include "AbstractLeaf.hpp"

RT::AbstractLeaf::AbstractLeaf()
{}

RT::AbstractLeaf::~AbstractLeaf()
{}

std::list<RT::Intersection>	RT::AbstractLeaf::render(RT::Ray const & ray) const
{
  // Get intersections distances
  std::vector<double>		dist = intersection(ray);

  // Return immedialty if no intersection
  if (dist.empty())
    return std::list<RT::Intersection>();

  // Get normal for each intersection
  std::list<RT::Intersection>	result;
  for (double it : dist)
  {
    RT::Ray			norm;

    // Calculate normal from intersection list
    norm.p() = ray.p() + ray.d() * it;
    norm.d() = normal(norm.p());
    
    // Push intersection data to intersections list
    result.push_back(RT::Intersection(this, norm.normalize(), it));
  }

  // Sort intersection according to distance
  result.sort();

  return result;
}
