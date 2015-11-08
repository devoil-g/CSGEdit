#ifndef _ABSTRACT_LIGHT_HPP_
#define _ABSTRACT_LIGHT_HPP_

#include "AbstractTree.hpp"
#include "Material.hpp"
#include "Math.hpp"

namespace RT
{
  class AbstractLight
  {
  public:
    AbstractLight();
    virtual ~AbstractLight();

    virtual RT::Color preview(AbstractTree const *, Math::Ray const &, Math::Ray const &, RT::Material const &) const = 0; // Return preview lighting for given location/material
    virtual RT::Color render(AbstractTree const *, Math::Ray const &, Math::Ray const &, RT::Material const &) const = 0;  // Return rendered lighting for given location/material
  };
};

#endif