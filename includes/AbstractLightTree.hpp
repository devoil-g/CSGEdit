#ifndef _ABSTRACT_LIGHT_TREE_HPP_
#define _ABSTRACT_LIGHT_TREE_HPP_

#include "Color.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"

namespace RT
{
  class Scene;

  class AbstractLightTree
  {
  public:
    AbstractLightTree();
    virtual ~AbstractLightTree();

    virtual RT::Color	preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int = 0) const = 0;	// Return preview lighting for given intersection
    virtual RT::Color	render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int = 0) const = 0;	// Return rendered lighting for given intersection
  };
};

#endif
