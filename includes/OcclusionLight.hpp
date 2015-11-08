#ifndef _OCCLUSION_LIGHT_HPP_
#define _OCCLUSION_LIGHT_HPP_

#include "AbstractLight.hpp"
#include "Color.hpp"
#include "Config.hpp"
#include "Math.hpp"

namespace RT
{
  class OcclusionLight : public AbstractLight
  {
  private:
    RT::Color	  _color;     // Color of the light.
    double	  _radius;    // Light radius. 0 for infinite.
    unsigned int  _quality;   // Quality of the light

  public:
    OcclusionLight(RT::Color const & = RT::Color(1.f), double = RT::Config::Light::DefaultAmbientRadius, unsigned int = RT::Config::Light::DefaultQuality);
    ~OcclusionLight();

    RT::Color	  preview(RT::AbstractTree const *, Math::Ray const &, Math::Ray const &, RT::Material const &) const override;
    RT::Color	  render(RT::AbstractTree const *, Math::Ray const &, Math::Ray const &, RT::Material const &) const override;
  };
};

#endif