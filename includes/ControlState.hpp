#ifndef _CONTROL_STATE_HPP_
#define _CONTROL_STATE_HPP_

#include <SFML/System/Time.hpp>
#include <string>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include "AbstractState.hpp"
#include "PreviewRaytracer.hpp"
#include "Scene.hpp"

namespace RT
{
  class ControlState : public AbstractState
  {
  private:

#ifdef WIN32
    typedef FILETIME t_FileTime;
#else
    typedef time_t   t_FileTime;
#endif

    RT::PreviewRaytracer	_preview;	// Raytracer for preview
    RT::Scene *			_scene;		// Current scene loaded
    std::string			_file;		// Path to current scene file
    t_FileTime			_time;		// Update time of current file scene

  private:
    bool  			updateTime();	// Check if current file has been modified

  public:
    ControlState(std::string const &);
    ~ControlState();

    bool  			update(sf::Time) override;	// Wait for input
    void  			draw() override;		// Display raytracer image
  };
};

#endif