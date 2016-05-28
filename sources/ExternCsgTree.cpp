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

size_t				RT::ExternCsgTree::build(std::vector<RT::OpenCL::Node> & nodes, std::vector<RT::OpenCL::Primitive> & primitives, Math::Matrix<4, 4> const & transformation, RT::Material const & material, unsigned int deph) const
{
  if (deph < RT::Config::Csg::MaxDeph)
    return (*_tree)->build(nodes, primitives, transformation, material, deph + 1);
  else
  {
    nodes.push_back(RT::OpenCL::Node());
    nodes.back().type = RT::OpenCL::Node::Type::TypeEmpty;

    return nodes.size() - 1;
  }
}
