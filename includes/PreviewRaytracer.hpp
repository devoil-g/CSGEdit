#ifndef _PREVIEW_RAYTRACER_HPP_
#define _PREVIEW_RAYTRACER_HPP_

#include <vector>

#include "AbstractRaytracer.hpp"
#include "Color.hpp"
#include "Scene.hpp"

namespace RT
{
  class PreviewRaytracer : public RT::AbstractRaytracer
  {
  private:
    std::vector<unsigned int>	_grid;	// Progression of scene to render
    RT::Scene *			_scene;	// Scene to render

    void			begin() override;				// Method managing rendering threads
    void			preview();					// Rendering thread main method
    void			preview(unsigned int);				// Render a zone
    RT::Color			preview(unsigned int, unsigned int) const;	// Render a pixel

  public:
    PreviewRaytracer();
    ~PreviewRaytracer();

    void			load(RT::Scene *) override;	// Load a new scene
    double			progress() const override;	// Return current progress (0-1)
  };
};

#endif
