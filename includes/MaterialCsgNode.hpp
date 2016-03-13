#ifndef _MATERIAL_CSG_NODE_HPP_
#define _MATERIAL_CSG_NODE_HPP_

#include <list>
#include <string>

#include "AbstractCsgNode.hpp"
#include "Intersection.hpp"
#include "Material.hpp"
#include "Ray.hpp"

namespace RT
{
  class MaterialCsgNode : public RT::AbstractCsgNode
  {
  private:
    RT::Material const		_material;	// Material applied to sub-node
    
    std::list<RT::Intersection>	renderChildren(RT::Ray const &, unsigned int) const override;	// Render sub-tree

  public:
    MaterialCsgNode(RT::Material const &);
    ~MaterialCsgNode();
  };
};

#endif
