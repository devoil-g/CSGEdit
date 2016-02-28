#ifndef _OCCLUSION_LIGHT_HPP_
#define _OCCLUSION_LIGHT_HPP_

#include "AbstractLight.hpp"
#include "Color.hpp"

namespace RT
{
  class OcclusionLight : public AbstractLight
  {
  private:
    RT::Color		_color;		// Color of the light.
    double		_radius;	// Light radius. 0 for infinite.

  public:
    OcclusionLight(RT::Color const & = RT::Color(1.f), double = 0.f);
    ~OcclusionLight();

    RT::Color		preview(RT::Scene const *, RT::Ray const &, RT::Intersection const &) const override;	// Render preview mode of light
    RT::Color		render(RT::Scene const *, RT::Ray const &, RT::Intersection const &) const override;	// Render complete light

    std::string		dump() const override;	// Dump light in string
  };
};

#endif