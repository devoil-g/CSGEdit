#include "EmptyCsgTree.hpp"

RT::EmptyCsgTree::EmptyCsgTree()
{}

RT::EmptyCsgTree::~EmptyCsgTree()
{}

std::list<RT::Intersection>	RT::EmptyCsgTree::render(RT::Ray const &, unsigned int) const
{
  return std::list<RT::Intersection>();
}
