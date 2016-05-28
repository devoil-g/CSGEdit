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

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;															// Render sub-tree
    size_t			build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const override;	// Build OpenCL data structure

    // Setter/getter of material
    RT::Material &		material() { return _material; };
    RT::Material const &	material() const { return _material; };
  };
};

#endif
