#ifndef _MATERIAL_NODE_HPP_
#define _MATERIAL_NODE_HPP_

#include <list>

#include "AbstractNode.hpp"
#include "Color.hpp"
#include "Intersection.hpp"
#include "Material.hpp"
#include "Matrix.hpp"

namespace RT
{
  class MaterialNode : public RT::AbstractNode
  {
  private:
    RT::Material const	_material;
    
    std::list<RT::Intersection>	renderChildren(Math::Ray const &) const override;  // Render sub-tree

  public:
    MaterialNode(RT::Material const &);
    ~MaterialNode();

    std::string			dump() const override;
  };
};

#endif