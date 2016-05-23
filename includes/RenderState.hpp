#ifndef _RENDER_STATE_HPP_
#define _RENDER_STATE_HPP_

#include <SFML/System/Time.hpp>

#include "AbstractState.hpp"
#include "AbstractRenderer.hpp"
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
    sf::Time			_elapsed;	// Elapsed time to calculate remaining time
    RT::AbstractRenderer *	_renderer;	// Basic renderer
    RT::Scene *			_scene;		// Current scene
    double			_wait;		// Time before progress update

  public:
    RenderState(RT::AbstractRenderer *, RT::Scene *);
    ~RenderState();

    bool			update(sf::Time) override;	// Update progress in rendering
    void			draw() override;		// Display image
  };
};

#endif
