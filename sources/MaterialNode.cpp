#include "MaterialNode.hpp"

RT::MaterialNode::MaterialNode(RT::Material const & material)
  : _material(material)
{}

RT::MaterialNode::MaterialNode(Math::Matrix<4, 4> const & transformation, RT::Material const & material)
  : AbstractNode(transformation), _material(material)
{}

RT::MaterialNode::~MaterialNode()
{}

std::list<RT::Intersection> RT::MaterialNode::renderTree(Math::Ray const & ray) const
{
  std::list<RT::Intersection> intersect;

  // Iterate through sub-tree to get intersections
  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    std::list<RT::Intersection>	node = (*it)->render(ray);

    // Atribute intersections and apply material to children
    for (std::list<RT::Intersection>::iterator it_node = node.begin(); it_node != node.end(); it_node++)
    {
      it_node->node = *it;
      it_node->material = it_node->material * _material;
    }

    intersect.merge(node);
  }

  std::map<RT::AbstractTree const *, bool>  inside;
  std::list<RT::Intersection>		    result;
  unsigned int				    state = 0;

  // Set all positions to 'outside' (false)
  for (std::list<RT::Intersection>::const_iterator iter = intersect.begin(); iter != intersect.end(); iter++)
    inside[iter->node] = false;

  // Iterate through intersections
  for (std::list<RT::Intersection>::const_iterator iter = intersect.begin(); iter != intersect.end(); iter++)
  {
    // If currently outside, push intersection
    if (state == 0)
      result.push_back(*iter);

    // Increment deepness if getting inside an object, decrement if getting outside
    if (inside[iter->node])
      state--;
    else
      state++;
    inside[iter->node] = !(inside[iter->node]);

    // If currently outside, push intersection
    if (state == 0)
      result.push_back(*iter);
  }

  return result;
}