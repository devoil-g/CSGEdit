#include "IntersectionCsgNode.hpp"

RT::IntersectionCsgNode::IntersectionCsgNode()
{}

RT::IntersectionCsgNode::~IntersectionCsgNode()
{}

std::list<RT::Intersection>	RT::IntersectionCsgNode::renderChildren(RT::Ray const & ray, unsigned int deph) const
{
  std::list<RT::Intersection>	result = _children.front()->render(ray, deph);

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractCsgTree *>::const_iterator it = std::next(_children.begin()); result.empty() == false && it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(ray, deph);

    if (node.empty())
      return std::list<RT::Intersection>();

    // Merge current node with final results
    std::list<RT::Intersection>::iterator	it_r = result.begin(), it_n = node.begin();
    bool					inside_r = false, inside_n = false;

    while (it_r != result.end() && it_n != node.end())
      if (*it_r < *it_n)
      {
	inside_r = !inside_r;
	if (inside_n == false)
	  result.erase(it_r++);
	else
	  it_r++;
      }
      else
      {
	inside_n = !inside_n;
	if (inside_r == true)
	  result.splice(it_r, node, it_n++);
	else
	  it_n++;
      }

    // Erase remaining intersection
    result.erase(it_r, result.end());
  }

  return result;
}
