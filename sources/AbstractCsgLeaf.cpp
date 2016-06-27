#include <algorithm>

#include "AbstractCsgLeaf.hpp"

RT::AbstractCsgLeaf::AbstractCsgLeaf()
{}

RT::AbstractCsgLeaf::~AbstractCsgLeaf()
{}

std::list<RT::Intersection>	RT::AbstractCsgLeaf::render(RT::Ray const & ray, unsigned int) const
{
  // Get intersections distances
  std::vector<double>		dist = intersection(ray);

  // Return immedialty if no intersection
  if (dist.empty())
    return std::list<RT::Intersection>();

  // Sort intersection according to distance
  std::sort(dist.begin(), dist.end());

  // Get normal for each intersection
  std::list<RT::Intersection>	result;
  for (double it : dist)
  {
    RT::Ray			norm;

    // Calculate normal from intersection list
    norm.p() = ray.p() + ray.d() * it;
    norm.d() = normal(norm.p());

    // Push intersection data to intersections list
    result.push_back(RT::Intersection(norm, it));
  }

  return result;
}
