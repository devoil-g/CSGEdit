#include "UnionCsgNode.hpp"

RT::UnionCsgNode::UnionCsgNode()
{}

RT::UnionCsgNode::~UnionCsgNode()
{}

std::list<RT::Intersection>	RT::UnionCsgNode::renderChildren(RT::Ray const & ray, unsigned int deph) const
{
  std::list<RT::Intersection>	result;

  // Iterate through sub-tree to get intersections
  for (RT::AbstractCsgTree const * it : children())
  {
    std::list<RT::Intersection>	node = it->render(ray, deph);
    
    // Merge current node with final result
    std::list<RT::Intersection>::iterator	it_r = result.begin(), it_n = node.begin();
    bool					inside_r = false, inside_n = false;

    while (it_r != result.end() && it_n != node.end())
      if (*it_r < *it_n)
      {
	inside_r = !inside_r;
	if (inside_n == true)
	  result.erase(it_r++);
	else
	  it_r++;
      }
      else
      {
	inside_n = !inside_n;
	if (inside_r == false)
	  result.splice(it_r, node, it_n++);
	else
	  it_n++;
      }

    // Insert remaining intersections
    result.splice(it_r, node, it_n, node.end());
  }

  return result;
}
