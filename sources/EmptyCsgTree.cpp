#include "EmptyCsgTree.hpp"

RT::EmptyCsgTree::EmptyCsgTree()
{}

RT::EmptyCsgTree::~EmptyCsgTree()
{}

std::list<RT::Intersection>	RT::EmptyCsgTree::render(RT::Ray const &, unsigned int) const
{
  return std::list<RT::Intersection>();
}

size_t				RT::EmptyCsgTree::build(std::vector<RT::OpenCL::Node> & nodes, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int) const
{
  nodes.push_back(RT::OpenCL::Node());
  nodes.back().type = RT::OpenCL::Node::Type::TypeEmpty;

  return nodes.size() - 1;
}
