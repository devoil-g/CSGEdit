#ifndef _RENDER_STATE_HPP_
#define _RENDER_STATE_HPP_

#include <SFML/System/Time.hpp>

#include "AbstractState.hpp"
#include "Raytracer.hpp"

namespace RT
{
  class RenderState : public AbstractState
  {
  private:
    sf::Time		  _elapsed;   // Elapsed time to calculate remaining time
    sf::Texture		  _texture;   // Texture generated from raytracer image
    sf::Sprite		  _sprite;    // Sprite generated from texture
    RT::Raytracer * const _raytracer; // Pointer to raytracer
    double		  _wait;      // Time before progress update

  public:
    RenderState(RT::Raytracer *);
    ~RenderState();

    bool  update(sf::Time) override;  // Update progress in rendering
    void  draw() override;	      // Display image
  };
};

#endif