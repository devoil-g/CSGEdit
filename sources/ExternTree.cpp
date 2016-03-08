#include "Exception.hpp"
#include "ExternTree.hpp"

RT::ExternTree::ExternTree(RT::AbstractTree const * const & tree)
  : _tree(tree)
{
  // Crash if invalid tree
  if (_tree == nullptr)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}

RT::ExternTree::~ExternTree()
{}

std::list<RT::Intersection>	RT::ExternTree::render(RT::Ray const & ray) const
{
  return _tree->render(ray);
}

std::string			RT::ExternTree::dump() const
{
  return _tree->dump();
}