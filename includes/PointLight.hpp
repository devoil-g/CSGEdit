#ifndef _POINT_LIGHT_HPP_
#define _POINT_LIGHT_HPP_

#include "AbstractLight.hpp"
#include "Color.hpp"
#include "Matrix.hpp"
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
    
  public:
    PointLight(Math::Matrix<4, 4> const &, RT::Color const & = RT::Color(1.f), double = 0.f, double = 0.f, double = 0.f, double = 0.f);
    ~PointLight();

    RT::Color		preview(RT::Scene const *, RT::Ray const &, RT::Intersection const &) const override;	// Render preview mode of light
    RT::Color		render(RT::Scene const *, RT::Ray const &, RT::Intersection const &) const override;	// Render complete light

    std::string		dump() const override;	// Dump light in string
  };
};

#endif