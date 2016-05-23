#ifndef _PAUSE_STATE_HPP_
#define _PAUSE_STATE_HPP_

#include <SFML/Graphics/Image.hpp>

#include "AbstractState.hpp"
#include "AbstractRenderer.hpp"
#include "Scene.hpp"

namespace RT
{
  class PauseState : public RT::AbstractState
  {
  private:
    sf::Image			_image;		// Copy of raytracer image
    RT::AbstractRenderer *	_renderer;	// Current render raytracer

  public:
    PauseState(RT::AbstractRenderer *, RT::Scene *, sf::Time);
    ~PauseState();

    bool			update(sf::Time) override;	// Check for P or ESC
    void			draw() override;		// Display sprite
  };
};

#endif
