#ifndef _CONTROL_STATE_HPP_
#define _CONTROL_STATE_HPP_

#include <SFML/System/Time.hpp>
#include <string>
#include <map>

#include "AbstractState.hpp"
#include "FileTime.hpp"
#include "Matrix.hpp"
#include "PreviewRaytracer.hpp"
#include "Scene.hpp"

namespace RT
{
  class ControlState : public AbstractState
  {
  private:

#ifdef _WIN32
    typedef FILETIME Time;	// Time structure (Windows, contain creation, last access & last write data)
#else
    typedef time_t   Time;	// Time structure (linux, time since Epoch in seconds)
#endif

    RT::PreviewRaytracer	_preview;	// Raytracer for preview
    RT::Scene *			_scene;		// Current scene loaded
    RT::FileTime		_file;		// FileTime of current loaded file
    Math::Matrix<4, 4>		_camera;	// Current position of camera

  private:
    bool  			updateFiles();				// Check if current file and dependencies have been modified
    
  public:
    ControlState(std::string const &);
    ~ControlState();

    bool  			update(sf::Time) override;	// Wait for input
    void  			draw() override;		// Display raytracer image
  };
};

#endif