#include "AbstractLeaf.hpp"

RT::AbstractLeaf::AbstractLeaf()
{}

RT::AbstractLeaf::~AbstractLeaf()
{}

std::list<RT::Intersection>	RT::AbstractLeaf::render(Math::Ray const & ray) const
{
  std::list<RT::Intersection>   result;
  std::vector<double>		dist;
  Math::Ray			norm;

  // Get intersections distances
  dist = intersection(ray);

  for (std::vector<double>::const_iterator it = dist.begin(); it != dist.end(); it++)
  {
    // Calculate normal from intersection list
    norm.px() = ray.px() + ray.dx() * (*it);
    norm.py() = ray.py() + ray.dy() * (*it);
    norm.pz() = ray.pz() + ray.dz() * (*it);
    norm.d() = normal(norm).d();
    norm = norm.normalize();

    // Push intersection data to intersections list
    result.push_back(RT::Intersection(norm, (*it)));
  }

  // Sort intersection according to distance
  result.sort();

  return result;
}
