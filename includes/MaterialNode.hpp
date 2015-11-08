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

    std::list<RT::Intersection>	renderTree(Math::Ray const &) const override;

  public:
    MaterialNode(RT::Material const &);
    MaterialNode(Math::Matrix<4, 4> const &, RT::Material const &);
    ~MaterialNode();
  };
};

#endif