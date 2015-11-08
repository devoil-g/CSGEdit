#ifndef _RENDER_PAUSE_STATE_HPP_
#define _RENDER_PAUSE_STATE_HPP_

#include "AbstractState.hpp"
#include "Raytracer.hpp"

namespace RT
{
  class RenderPauseState : public AbstractState
  {
  private:
    sf::Image	    _image;	// Copy of raytracer image
    sf::Sprite	    _sprite;	// Sprite generated from raytracer image
    sf::Texture	    _texture;	// Texture generated from sprite
    RT::Raytracer * _raytracer;	// Pointer to raytracer

  public:
    RenderPauseState(RT::Raytracer *);
    ~RenderPauseState();

    bool  update(sf::Time) override;  // Check for P or ESC
    void  draw() override;	      // Display sprite
  };
};

#endif
