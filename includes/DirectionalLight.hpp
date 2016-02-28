#ifndef _DIRECTIONAL_LIGHT_HPP_
#define _DIRECTIONAL_LIGHT_HPP_

#include "AbstractLight.hpp"
#include "Color.hpp"
#include "Config.hpp"
#include "Material.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"

namespace RT
{
  class DirectionalLight : public AbstractLight
  {
  private:
    RT::Ray		_position;	// Light position and direction.
    RT::Color		_color;		// Color of the light.
    double		_angle;		// Light diffusion angle.
    
  public:
    DirectionalLight(Math::Matrix<4, 4> const &, RT::Color const & = RT::Color(1.f), double = 0.f);
    ~DirectionalLight();

    RT::Color		preview(RT::Scene const *, RT::Ray const &, RT::Intersection const &) const override;	// Render preview mode
    RT::Color		render(RT::Scene const *, RT::Ray const &, RT::Intersection const &) const override;	// Render complete light

    std::string		dump() const override;	// Dump light in string
  };
};

#endif