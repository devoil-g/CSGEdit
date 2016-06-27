#include "ExternCsgTree.hpp"

RT::ExternCsgTree::ExternCsgTree(RT::AbstractCsgTree const * const * tree)
  : _tree(tree)
{}

RT::ExternCsgTree::~ExternCsgTree()
{}

std::list<RT::Intersection>	RT::ExternCsgTree::render(RT::Ray const & ray, unsigned int deph) const
{
  if (deph < RT::Config::Csg::MaxDeph)
    return (*_tree)->render(ray, deph + 1);
  else
    return std::list<RT::Intersection>();
}
