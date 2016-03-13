#ifndef _POINT_LIGHT_LEAF_HPP_
#define _POINT_LIGHT_LEAF_HPP_

#include "AbstractLightLeaf.hpp"
#include "Color.hpp"

namespace RT
{
  class PointLightLeaf : public RT::AbstractLightLeaf
  {
  private:
    RT::Color	_color;			// Color of the light.
    double	_radius;		// Light radius.
    double	_intensity;		// Light intensity multiplier. 0 for constant.
    double	_angle1, _angle2;	// Spot diffusion angles. 
    
  public:
    PointLightLeaf(RT::Color const & = RT::Color(1.f), double = 0.f, double = 0.f, double = 0.f, double = 0.f);
    ~PointLightLeaf();

    RT::Color	preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int) const override;	// Render preview mode of light
    RT::Color	render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int) const override;	// Render complete light
  };
};

#endif
