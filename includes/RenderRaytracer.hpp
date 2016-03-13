#ifndef _RENDER_RAYTRACER_HPP_
#define _RENDER_RAYTRACER_HPP_

#include <vector>

#include "AbstractRaytracer.hpp"
#include "Color.hpp"
#include "Intersection.hpp"
#include "Scene.hpp"

namespace RT
{
  class RenderRaytracer : public RT::AbstractRaytracer
  {
  private:
    enum Pass
    {
      First,
      Second
    };

    std::vector<unsigned int>	_grid;		// Progression of scene to render
    std::vector<unsigned int>	_antialiasing;	// Post anti-alistd::asing level
    RT::Scene *			_scene;		// Scene to render
    unsigned int		_progress;	// Number of ray traced during first pass
    Pass			_status;	// Status of pass (first or second)

    void    			antialiasing();	// Render post anti-alistd::asing level

    void    			begin() override;									// Method managing rendering threads
    void    			render();										// Rendering thread main method
    void    			render(unsigned int);									// Render a zone
    RT::Color			renderAntialiasing(unsigned int, unsigned int, unsigned int) const;			// Render antialistd::asing
    RT::Color			renderAnaglyph3D(RT::Ray const &) const;						// Render anaglyph image using parameters stored in RT::Config::Anaglyph3D
    RT::Color			renderDephOfField(RT::Ray const &) const;						// Render deph of field image using parameters stored in RT::Config::DephOfField
    RT::Color			renderRay(RT::Ray const &, unsigned int = 0) const;					// Render a ray with CSG tree
    RT::Color			renderReflection(RT::Ray const &, RT::Intersection const &, unsigned int) const;	// Render reflection of ray  
    RT::Color			renderTransparency(RT::Ray const &, RT::Intersection const &, unsigned int) const;	// Render transparency
    RT::Color			renderLight(RT::Ray const &, RT::Intersection const &, unsigned int) const;		// Render light of an intersection

  public:
    RenderRaytracer();
    ~RenderRaytracer();

    void    			load(RT::Scene *) override;	// Load a new scene
    double  			progress() const override;	// Return current progress (0-1)
  };
};

#endif
