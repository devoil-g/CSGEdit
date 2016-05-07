#ifndef _MATERIAL_CSG_NODE_HPP_
#define _MATERIAL_CSG_NODE_HPP_

#include "UnionCsgNode.hpp"
#include "Material.hpp"

namespace RT
{
  class MaterialCsgNode : public RT::UnionCsgNode
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
