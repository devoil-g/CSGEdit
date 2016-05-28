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
	unsigned int const	MaxSample(16);	// Maximum sample per pixel (2^n)
	unsigned int const	MaxBounce(8);	// Maximum of ray bounce
      };
    };
  };

  class AdvancedRenderer : public RT::AbstractRenderer
  {
  private:
    std::vector<unsigned int>	_grid;		// Number of sample per pixel for current zone (2^n)
    RT::Scene *			_scene;		// Scene to render
    unsigned long		_ray;		// Number of sample rendered
    unsigned long		_sample;	// Current level of sampling

    void	begin() override;				// Method managing rendering threads
    void	render();					// Rendering thread main method
    void	render(unsigned int);				// Render a zone
    RT::Color	renderVirtualReality(RT::Ray const &) const;	// Render virtual reality image
    RT::Color	renderDephOfField(RT::Ray const &) const;	// Render deph of field image
    RT::Color	renderRay(RT::Ray const &) const;		// Render a ray with CSG tree
    
  public:
    AdvancedRenderer();
    ~AdvancedRenderer();

    void	load(RT::Scene *) override;	// Load a new scene
    double	progress() override;		// Return current progress (0-1)
  };
};

#endif
