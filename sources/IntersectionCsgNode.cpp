#include "IntersectionCsgNode.hpp"

RT::IntersectionCsgNode::IntersectionCsgNode()
{}

RT::IntersectionCsgNode::~IntersectionCsgNode()
{}

std::list<RT::Intersection>	RT::IntersectionCsgNode::renderChildren(RT::Ray const & ray, unsigned int deph) const
{
  std::list<RT::Intersection>	result = _children.front()->render(ray, deph);

  if (result.empty())
    return std::list<RT::Intersection>();

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractCsgTree *>::const_iterator it = std::next(_children.begin()); it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(ray, deph);

    if (node.empty())
      return std::list<RT::Intersection>();

    // Merge current node with final results
    std::list<RT::Intersection>::const_iterator	it_r = result.begin(), it_n = node.begin();
    bool					inside_r = false, inside_n = false;

    while (it_r != result.end() && it_n != node.end())
      if (*it_r < *it_n)
      {
	inside_r = !inside_r;
	if (inside_n == false)
	  it_r = result.erase(it_r);
	else
	  it_r++;
      }
      else
      {
	inside_n = !inside_n;
	if (inside_r == true)
	  result.insert(it_r, *it_n);
	it_n++;
      }

    // Erase remaining intersection
    result.erase(it_r, result.end());

    if (result.empty())
      return std::list<RT::Intersection>();
  }

  return result;
}
