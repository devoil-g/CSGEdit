#include "AbstractLeaf.hpp"

RT::AbstractLeaf::AbstractLeaf()
{}

RT::AbstractLeaf::~AbstractLeaf()
{}

std::list<RT::Intersection>	RT::AbstractLeaf::render(RT::Ray const & ray) const
{
  std::list<RT::Intersection>   result;
  RT::Ray			norm;

  // Get intersections distances
  std::vector<double>		dist = intersection(ray);

  for (std::vector<double>::const_iterator it = dist.begin(); it != dist.end(); it++)
  {
    // Calculate normal from intersection list
    norm.p() = ray.p() + ray.d() * (*it);
    norm.d() = normal(norm.p());
    norm = norm.normalize();

    // Push intersection data to intersections list
    result.push_back(RT::Intersection(norm, (*it)));
  }

  // Sort intersection according to distance
  result.sort();

  return result;
}
