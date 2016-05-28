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

size_t				RT::AbstractCsgLeaf::build(std::vector<RT::OpenCL::Node> & nodes, std::vector<RT::OpenCL::Primitive> & primitives, Math::Matrix<4, 4> const & transformation, RT::Material const & material, unsigned int) const
{
  nodes.push_back(RT::OpenCL::Node());
  primitives.push_back(RT::OpenCL::Primitive());

  nodes.back().type = RT::OpenCL::Node::Type::TypePrimitive;
  nodes.back().data.primitive.index = (int)primitives.size() - 1;

  Math::Matrix<4, 4>		matrix = transformation.inverse();

  for (unsigned row = 0; row < 4; row++)
    for (unsigned col = 0; col < 4; col++)
      primitives.back().transformation[row * 4 + col] = (float)matrix(row, col);

  primitives.back().material.color[0] = (float)material.color.r;
  primitives.back().material.color[1] = (float)material.color.g;
  primitives.back().material.color[2] = (float)material.color.b;

  return nodes.size() - 1;
}