#ifndef _RENDER_STATE_HPP_
#define _RENDER_STATE_HPP_

#include "AbstractState.hpp"
#include "RenderRaytracer.hpp"
#include "Scene.hpp"

namespace RT
{
  namespace Config
  {
    namespace RenderState
    {
      double const	Refresh(0.2f);
    };
  };

  class RenderState : public RT::AbstractState
  {
  private:
    sf::Time		_elapsed;	// Elapsed time to calculate remaining time
    RT::RenderRaytracer	_render;	// Pointer to raytracer
    RT::Scene *		_scene;		// Current scene
    double		_wait;		// Time before progress update

  public:
    RenderState(RT::Scene *);
    ~RenderState();

    bool  		update(sf::Time) override;	// Update progress in rendering
    void  		draw() override;		// Display image
  };
};

#endif
