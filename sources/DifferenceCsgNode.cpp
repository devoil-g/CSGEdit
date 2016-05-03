#include "DifferenceCsgNode.hpp"

RT::DifferenceCsgNode::DifferenceCsgNode()
{}

RT::DifferenceCsgNode::~DifferenceCsgNode()
{}

std::list<RT::Intersection>	RT::DifferenceCsgNode::renderChildren(RT::Ray const & ray, unsigned int deph) const
{
  std::list<RT::Intersection>	result = _children.front()->render(ray, deph);

  if (result.empty())
    return std::list<RT::Intersection>();

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractCsgTree *>::const_iterator it = std::next(_children.begin()); it != _children.end(); it++)
  {
    std::list<RT::Intersection>	node = (*it)->render(ray, deph);

    // Merge current node with final results
    std::list<RT::Intersection>::const_iterator	it_r = result.begin();
    std::list<RT::Intersection>::iterator	it_n = node.begin();
    bool					inside_r = false, inside_n = false;

    while (it_r != result.end() && it_n != node.end())
      if (*it_r < *it_n)
      {
	inside_r = !inside_r;
	if (inside_n == true)
	  it_r = result.erase(it_r);
	else
	  it_r++;
      }
      else
      {
	inside_n = !inside_n;
	if (inside_r == true)
	{
	  // Reverse normal of negative objects
	  it_n->normal.d() *= -1.f;
	  result.insert(it_r, *it_n);
	}
	it_n++;
      }

    if (result.empty())
      return std::list<RT::Intersection>();
  }

  return result;
}
