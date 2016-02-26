#include <map>

#include "DifferenceNode.hpp"

RT::DifferenceNode::DifferenceNode()
{}

RT::DifferenceNode::~DifferenceNode()
{}

std::list<RT::Intersection>	RT::DifferenceNode::renderChildren(RT::Ray const & ray) const
{
  if (_children.empty())
    return std::list<RT::Intersection>();

  std::list<RT::Intersection>			uni = _children.front()->render(ray);

  if (uni.empty())
    return std::list<RT::Intersection>();

  std::list<RT::Intersection>			dif;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree *>::const_iterator it = std::next(_children.begin()); it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(ray);

    // Atribute intersections to children
    for (std::list<RT::Intersection>::iterator it_node = node.begin(); it_node != node.end(); it_node++)
    {
      // Reverse normal (for refraction)
      it_node->normal.d() *= -1.f;
      it_node->node = *it;
    }

    dif.merge(node);
  }

  std::map<RT::AbstractTree const *, bool>	inside;
  std::list<RT::Intersection>			result;
  unsigned int					state_uni = 0, state_dif = 0;

  std::list<RT::Intersection>::iterator		it_uni = uni.begin(), it_dif = dif.begin();

  // Iteration through positive object intersections
  while (it_uni != uni.end())
  {
    // If before next negative object
    if (it_dif == dif.end() || it_uni->distance < it_dif->distance)
    {
      // If outside negative object
      if (state_dif == 0)
	result.push_back(*it_uni);

      // Reverse inside/outside of positive object
      state_uni = state_uni == 1 ? 0 : 1;

      it_uni++;
    }
    // If after next negative object
    else
    {
      // If inside positive object and outside negative object
      if (state_uni != 0 && state_dif == 0)
	result.push_back(*it_dif);

      // Increment deepness if getting inside an object, decrement if getting outside
      if (inside[it_dif->node])
	state_dif--;
      else
	state_dif++;
      inside[it_dif->node] = !(inside[it_dif->node]);

      // If inside positive object and outside negative object
      if (state_uni != 0 && state_dif == 0)
	result.push_back(*it_dif);

      it_dif++;
    }
  }

  return result;
}

std::string	RT::DifferenceNode::dump() const
{
  std::stringstream stream;

  stream << "difference();";

  for (std::list<RT::AbstractTree *>::const_iterator it = _children.begin(); it != _children.end(); it++)
    stream << (*it)->dump();

  stream << "end();";

  return stream.str();
}