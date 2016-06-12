#ifndef _ADVANCED_RENDERER_HPP_
#define _ADVANCED_RENDERER_HPP_

#include <vector>

#include "AbstractRenderer.hpp"
#include "Color.hpp"
#include "Intersection.hpp"
#include "Scene.hpp"

namespace RT
{
  namespace Config
  {
    namespace Renderer
    {
      namespace Advanced
      {
	unsigned int const	MaxSample(16);		// Maximum sample per pixel (2^n)
	unsigned int const	MaxBounces(16);		// Maximum of ray bounce
	RT::Color const		MinMask(1.f / 255.f);	// Minimum color mask
      };
    };
  };

  class AdvancedRenderer : public RT::AbstractRenderer
  {
  private:
    std::vector<unsigned int>	_grid;		// Number of sample per pixel for current zone (2^n)
    std::vector<RT::Color>	_image;		// Image with true colors
    RT::Scene *			_scene;		// Scene to render
    unsigned long		_ray;		// Number of sample rendered
    unsigned long		_sample;	// Current level of sampling

    void	begin() override;										// Method managing rendering threads
    void	render();											// Rendering thread main method
    void	render(unsigned int);										// Render a zone
    RT::Color	renderVirtualReality(RT::Ray const &) const;							// Render virtual reality image
    RT::Color	renderDephOfField(RT::Ray const &) const;							// Render deph of field image
    RT::Color	renderCamera(RT::Ray const &) const;								// Render scene camera
    RT::Color	renderRay(RT::Ray const &, RT::Color = RT::Color(1.f), unsigned int = 0) const;			// Render a ray with CSG tree    
    RT::Color	renderDiffuse(RT::Ray const &, RT::Intersection const &, RT::Color, unsigned int) const;	// Render diffuse bounces
    RT::Color	renderSpecular(RT::Ray const &, RT::Intersection const &, RT::Color, unsigned int) const;	// Render specular bounces
    RT::Color	renderReflection(RT::Ray const &, RT::Intersection const &, RT::Color, unsigned int) const;	// Render reflection bounces
    RT::Color	renderTransparency(RT::Ray const &, RT::Intersection const &, RT::Color, unsigned int) const;	// Render transparency bounces
    RT::Color	renderEmission(RT::Ray const &, RT::Intersection const &, RT::Color, unsigned int) const;	// Render emitted light

  public:
    AdvancedRenderer();
    ~AdvancedRenderer();

    void	load(RT::Scene *) override;	// Load a new scene
    double	progress() override;		// Return current progress (0-1)
  };
};

#endif
