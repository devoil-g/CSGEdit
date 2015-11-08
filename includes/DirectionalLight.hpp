#ifndef _DIRECTIONAL_LIGHT_HPP_
#define _DIRECTIONAL_LIGHT_HPP_

#include "AbstractLight.hpp"
#include "Color.hpp"
#include "Config.hpp"
#include "Matrix.hpp"

namespace RT
{
  class DirectionalLight : public AbstractLight
  {
  private:
    Math::Ray	  _position;  // Light position and direction.
    RT::Color	  _color;     // Color of the light.
    double	  _angle;     // Light diffusion angle.
    unsigned int  _quality;   // Quality of the light.

  public:
    DirectionalLight(Math::Matrix<4, 4> const &, RT::Color const & = RT::Color(1.f), double = 0, unsigned int = RT::Config::Light::DefaultQuality);
    ~DirectionalLight();

    RT::Color	  preview(RT::AbstractTree const *, Math::Ray const &, Math::Ray const &, RT::Material const &) const override;
    RT::Color	  render(RT::AbstractTree const *, Math::Ray const &, Math::Ray const &, RT::Material const &) const override;
  };
};

#endif