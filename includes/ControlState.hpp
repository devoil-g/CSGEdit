#ifndef _CONTROL_STATE_HPP_
#define _CONTROL_STATE_HPP_

#include <string>

#include "AbstractState.hpp"
#include "Matrix.hpp"
#include "PreviewRaytracer.hpp"
#include "Scene.hpp"

namespace RT
{
  class ControlState : public RT::AbstractState
  {
  private:
    RT::PreviewRaytracer	_preview;	// Raytracer for preview
    RT::Scene *			_scene;		// Current scene
    std::string			_file;		// Current file
    Math::Matrix<4, 4>		_camera;	// Camera applied to current scene

  public:
    ControlState(std::string const &);
    ~ControlState();

    bool  			update(sf::Time) override;	// Wait for input or scene change
    void  			draw() override;		// Display raytracer image
  };
};

#endif
