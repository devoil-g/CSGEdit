#ifndef _BASIC_RENDERER_HPP_
#define _BASIC_RENDERER_HPP_

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
      namespace Basic
      {
	unsigned int const	MaxRecursivite(3);	// Maximum of 'bounce' on a mirror
      };
    };
  };

  class BasicRenderer : public RT::AbstractRenderer
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
    unsigned long		_rayTotal;	// Total number of ray to render
    unsigned long		_rayRendered;	// Number of ray rendered
    Pass			_status;	// Status of pass (first or second)

    
    void	begin() override;									// Method managing rendering threads
    void	render();										// Rendering thread main method
    void	render(unsigned int);									// Render a zone
    void	antialiasing();										// Render post anti-aliasing level
    RT::Color	renderAntialiasing(unsigned int, unsigned int, unsigned int) const;			// Render antialiasing
    RT::Color	renderVirtualReality(RT::Ray const &) const;						// Render virtual reality image
    RT::Color	renderDephOfField(RT::Ray const &) const;						// Render deph of field image
    RT::Color	renderRay(RT::Ray const &, unsigned int = 0) const;					// Render a ray with CSG tree
    RT::Color	renderReflection(RT::Ray const &, RT::Intersection const &, unsigned int) const;	// Render reflection of ray  
    RT::Color	renderTransparency(RT::Ray const &, RT::Intersection const &, unsigned int) const;	// Render transparency
    RT::Color	renderLight(RT::Ray const &, RT::Intersection const &, unsigned int) const;		// Render direct light of an intersection
    
  public:
    BasicRenderer();
    ~BasicRenderer();

    void	load(RT::Scene *) override;	// Load a new scene
    double	progress() override;		// Return current progress (0-1)
  };
};

#endif
