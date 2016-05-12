#ifndef _MATERIAL_CSG_NODE_HPP_
#define _MATERIAL_CSG_NODE_HPP_

#include "UnionCsgNode.hpp"
#include "Material.hpp"

namespace RT
{
  class MaterialCsgNode : public RT::UnionCsgNode
  {
  private:
    RT::Material		_material;	// Material applied to sub-node

  public:
    MaterialCsgNode(RT::Material const &);
    ~MaterialCsgNode();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render sub-tree

    // Setter/getter of material
    RT::Material &		material() { return _material; };
    RT::Material const &	material() const { return _material; };
  };
};

#endif
