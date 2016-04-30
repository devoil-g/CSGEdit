#ifndef _OCCLUSION_LIGHT_LEAF_HPP_
#define _OCCLUSION_LIGHT_LEAF_HPP_

#include "AbstractLightLeaf.hpp"
#include "Color.hpp"

namespace RT
{
  class OcclusionLightLeaf : public RT::AbstractLightLeaf
  {
  private:
    RT::Color	_color;		// Color of the light.
    double	_radius;	// Light radius. 0 for infinite.

  public:
    OcclusionLightLeaf(RT::Color const & = RT::Color(1.f), double = 0.f);
    ~OcclusionLightLeaf();

    RT::Color	preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Render preview mode of light
    RT::Color	render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Render complete light
  };
};

#endif
