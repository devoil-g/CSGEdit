#ifndef _POINT_LIGHT_HPP_
#define _POINT_LIGHT_HPP_

#include "AbstractLight.hpp"
#include "AbstractTree.hpp"
#include "Color.hpp"
#include "Config.hpp"
#include "Material.hpp"
#include "Ray.hpp"

namespace RT
{
  class PointLight : public AbstractLight
  {
  private:
    RT::Ray		_position;		// Light position/direction.
    RT::Color		_color;			// Color of the light.
    double		_radius;		// Light radius.
    double		_intensity;		// Light intensity multiplier. 0 for constant.
    double		_angle1, _angle2;	// Spot diffusion angles. 
    unsigned int	_quality;		// Quality of the light

  public:
    PointLight(Math::Matrix<4, 4> const &, RT::Color const & = RT::Color(1.f), double = 0, double = 0, double = 0, double = 0, unsigned int = RT::Config::Light::Quality);
    ~PointLight();

    RT::Color		preview(RT::Scene const *, RT::Ray const &, RT::Ray const &, RT::Material const &) const override;	// Render preview mode of light
    RT::Color		render(RT::Scene const *, RT::Ray const &, RT::Ray const &, RT::Material const &) const override;	// Render complete light

    std::string		dump() const override;											// Dump light in string
  };
};

#endif