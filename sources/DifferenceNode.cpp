#include <map>

#include "DifferenceNode.hpp"

RT::DifferenceNode::DifferenceNode()
{}

RT::DifferenceNode::~DifferenceNode()
{}

std::list<RT::Intersection>	RT::DifferenceNode::renderChildren(Math::Ray const & ray) const
{
  std::list<std::list<RT::Intersection> > intersect_list;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(ray);

    // Atribute intersections to children
    for (std::list<RT::Intersection>::iterator it_node = node.begin(); it_node != node.end(); it_node++)
    {
      it_node->node = *it;

      // Reverse normal (for refraction)
      if (it != _children.begin())
	it_node->normal.d() = Math::Matrix<4, 4>::scale(-1.f) * it_node->normal.d();
    }

    // Acceleration tweak
    if (node.empty() && it == _children.begin())
      return std::list<RT::Intersection>();

    intersect_list.push_back(node);
  }

  std::map<RT::AbstractTree const *, bool>  inside;
  std::list<RT::Intersection>::iterator	    it_uni, it_dif;
  std::list<RT::Intersection>		    uni, dif, result;
  unsigned int				    state_uni = 0;
  unsigned int				    state_dif = 0;

  // Create positive and negative objects lists
  for (std::list<std::list<RT::Intersection> >::iterator iter = intersect_list.begin(); iter != intersect_list.end(); iter++)
  {
    if (iter == intersect_list.begin())
      uni.merge(*iter);
    else
      dif.merge(*iter);
  }

  // Set all positions to 'outside' (false)
  for (std::list<RT::Intersection>::iterator iter = dif.begin(); iter != dif.end(); iter++)
    inside[iter->node] = false;

  it_uni = uni.begin();
  it_dif = dif.begin();

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

  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
    stream << (*it)->dump();

  stream << "end();";

  return stream.str();
}