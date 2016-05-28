#include "UnionCsgNode.hpp"

RT::UnionCsgNode::UnionCsgNode()
{}

RT::UnionCsgNode::~UnionCsgNode()
{}

std::list<RT::Intersection>	RT::UnionCsgNode::render(RT::Ray const & ray, unsigned int deph) const
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

size_t				RT::UnionCsgNode::build(std::vector<RT::OpenCL::Node> & nodes, std::vector<RT::OpenCL::Primitive> & primitives, Math::Matrix<4, 4> const & transformation, RT::Material const & material, unsigned int deph) const
{
  if (children().size() == 1)
    return children().front()->build(nodes, primitives, transformation, material, deph);

  nodes.push_back(RT::OpenCL::Node());
  nodes.back().type = RT::OpenCL::Node::Type::TypeUnion;
  nodes.back().data.csg.left = (int)children().front()->build(nodes, primitives, transformation, material, deph);

  size_t			first_node = nodes.size() - 1;
  size_t			current_node = nodes.size() - 1;

  for (std::list<RT::AbstractCsgTree *>::const_iterator it = std::next(children().begin()); it != children().end(); it++)
    if (std::next(it) != children().end())
    {
      size_t			id = (*it)->build(nodes, primitives, transformation, material, deph);

      nodes.push_back(RT::OpenCL::Node());
      nodes.back().type = RT::OpenCL::Node::Type::TypeUnion;
      nodes.back().data.csg.left = (int)id;

      current_node = nodes.size() - 1;
    }
    else
      nodes[current_node].data.csg.right = (int)(*it)->build(nodes, primitives, transformation, material, deph);

  return first_node;
}
