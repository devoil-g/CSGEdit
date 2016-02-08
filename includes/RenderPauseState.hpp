#ifndef _RENDER_PAUSE_STATE_HPP_
#define _RENDER_PAUSE_STATE_HPP_

#include <SFML/Graphics/Image.hpp>
#include <SFML/System/Time.hpp>

#include "AbstractState.hpp"
#include "RenderRaytracer.hpp"
#include "Scene.hpp"

namespace RT
{
  class RenderPauseState : public AbstractState
  {
  private:
    sf::Image			_image;		// Copy of raytracer image
    RT::RenderRaytracer &	_render;	// Current render raytracer
    
  public:
    RenderPauseState(RT::RenderRaytracer &, RT::Scene *);
    ~RenderPauseState();

    bool			update(sf::Time) override;	// Check for P or ESC
    void			draw() override;		// Display sprite
  };
};

#endif
