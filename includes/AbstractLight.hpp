#ifndef _ABSTRACT_LIGHT_HPP_
#define _ABSTRACT_LIGHT_HPP_

#include <string>

#include "Color.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class Scene;

  class AbstractLight
  {
  public:
    AbstractLight();
    virtual ~AbstractLight();

    virtual RT::Color	preview(RT::Scene const *, RT::Ray const &, RT::Intersection const &) const = 0;	// Return preview lighting for given intersection
    virtual RT::Color	render(RT::Scene const *, RT::Ray const &, RT::Intersection const &) const = 0;		// Return rendered lighting for given intersection

    virtual std::string	dump() const = 0;	// Dump light in string
  };
};

#endif