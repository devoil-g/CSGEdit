#ifndef _DIRECTIONAL_LIGHT_LEAF_HPP_
#define _DIRECTIONAL_LIGHT_LEAF_HPP_

#include "AbstractLightLeaf.hpp"
#include "Color.hpp"

namespace RT
{
  class DirectionalLightLeaf : public RT::AbstractLightLeaf
  {
  private:
    RT::Color	_color;		// Color of the light.
    double	_angle;		// Light diffusion angle.
    
  public:
    DirectionalLightLeaf(RT::Color const & = RT::Color(1.f), double = 0.f);
    ~DirectionalLightLeaf();

    RT::Color	preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int) const override;	// Render preview mode
    RT::Color	render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int) const override;	// Render complete light
  };
};

#endif
