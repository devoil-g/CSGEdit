#ifndef _OCCLUSION_LIGHT_HPP_
#define _OCCLUSION_LIGHT_HPP_

#include "AbstractLight.hpp"
#include "AbstractTree.hpp"
#include "Color.hpp"
#include "Config.hpp"
#include "Material.hpp"

namespace RT
{
  class OcclusionLight : public AbstractLight
  {
  private:
    RT::Color		_color;		// Color of the light.
    double		_radius;	// Light radius. 0 for infinite.
    unsigned int	_quality;	// Quality of the light

  public:
    OcclusionLight(RT::Color const & = RT::Color(1.f), double = RT::Config::Light::AmbientRadius, unsigned int = RT::Config::Light::Quality);
    ~OcclusionLight();

    RT::Color		preview(RT::Scene const *, Math::Ray const &, Math::Ray const &, RT::Material const &) const override;	// Render preview mode of light
    RT::Color		render(RT::Scene const *, Math::Ray const &, Math::Ray const &, RT::Material const &) const override;	// Render complete light

    std::string		dump() const override;											// Dump light in string
  };
};

#endif