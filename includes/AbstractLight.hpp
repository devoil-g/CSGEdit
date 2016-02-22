#ifndef _ABSTRACT_LIGHT_HPP_
#define _ABSTRACT_LIGHT_HPP_

#include "AbstractTree.hpp"
#include "Material.hpp"
#include "Ray.hpp"

namespace RT
{
  class Scene;

  class AbstractLight
  {
  public:
    AbstractLight();
    virtual ~AbstractLight();

    virtual RT::Color	preview(RT::Scene const *, Math::Ray const &, Math::Ray const &, RT::Material const &) const = 0;	// Return preview lighting for given location/material
    virtual RT::Color	render(RT::Scene const *, Math::Ray const &, Math::Ray const &, RT::Material const &) const = 0;	// Return rendered lighting for given location/material

    virtual std::string	dump() const = 0;											// Dump light in string
  };
};

#endif