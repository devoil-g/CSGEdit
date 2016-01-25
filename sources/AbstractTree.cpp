#include "AbstractTree.hpp"

RT::AbstractTree::AbstractTree()
  : _transformation(Math::Matrix<4, 4>::identite())
{}

RT::AbstractTree::AbstractTree(Math::Matrix<4, 4> const & transformation)
  : _transformation(transformation)
{}

RT::AbstractTree::~AbstractTree()
{}

std::list<RT::Intersection>	RT::AbstractTree::render(Math::Ray const & ray) const
{
  std::list<RT::Intersection>	result;

  // Render intersections list according to transformed ray
  result = renderTree(_transformation.inverse() * ray);

  for (std::list<RT::Intersection>::iterator it = result.begin(); it != result.end(); it++)
  {
    // Inverse transformation
    (*it).normal.p() = _transformation * (*it).normal.p();
    (*it).normal.d() = _transformation.inverse().transpose() * (*it).normal.d();
    (*it).normal = (*it).normal.normalize();

    // Attribute top node
    (*it).node = this;
  }

  return result;
}

Math::Matrix<4, 4> const &	RT::AbstractTree::transformation() const
{
  return _transformation;
}