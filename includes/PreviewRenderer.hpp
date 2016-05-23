#ifndef _PREVIEW_RENDERER_HPP_
#define _PREVIEW_RENDERER_HPP_

#include <vector>

#include "AbstractRenderer.hpp"
#include "Color.hpp"
#include "Scene.hpp"

namespace RT
{
  class PreviewRenderer : public RT::AbstractRenderer
  {
  private:
    std::vector<unsigned int>	_grid;	// Progression of scene to render
    RT::Scene *			_scene;	// Scene to render

    void			begin() override;				// Method managing rendering threads
    void			preview();					// Rendering thread main method
    void			preview(unsigned int);				// Render a zone
    RT::Color			preview(unsigned int, unsigned int) const;	// Render a pixel

  public:
    PreviewRenderer();
    ~PreviewRenderer();

    void			load(RT::Scene *) override;	// Load a new scene
    double			progress() override;		// Return current progress (0-1)
  };
};

#endif
