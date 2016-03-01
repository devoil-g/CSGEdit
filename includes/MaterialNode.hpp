#ifndef _MATERIAL_NODE_HPP_
#define _MATERIAL_NODE_HPP_

#include <list>
#include <string>

#include "AbstractNode.hpp"
#include "Intersection.hpp"
#include "Material.hpp"
#include "Ray.hpp"

namespace RT
{
  class MaterialNode : public RT::AbstractNode
  {
  private:
    RT::Material const		_material;	// Material applied to sub-node
    
    std::list<RT::Intersection>	renderChildren(RT::Ray const &) const override;	// Render sub-tree

  public:
    MaterialNode(RT::Material const &);
    ~MaterialNode();

    std::string			dump() const override;	// Dump CSG tree
  };
};

#endif