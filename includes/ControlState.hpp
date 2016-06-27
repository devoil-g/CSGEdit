#ifndef _CONTROL_STATE_HPP_
#define _CONTROL_STATE_HPP_

#include <string>

#include "AbstractState.hpp"
#include "PreviewRenderer.hpp"
#include "Scene.hpp"

namespace RT
{
  class ControlState : public RT::AbstractState
  {
  private:
    RT::PreviewRenderer	_preview;	// Renderer for preview
    RT::Scene *		_scene;		// Current scene
    std::string		_file;		// Current file
    
  public:
    ControlState(std::string const &);
    ~ControlState();

    bool		update(sf::Time) override;	// Wait for input or scene change
    void		draw() override;		// Display raytracer image
  };
};

#endif
