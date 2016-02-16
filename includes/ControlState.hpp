#ifndef _CONTROL_STATE_HPP_
#define _CONTROL_STATE_HPP_

#include <SFML/System/Time.hpp>
#include <string>
#include <map>

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
    typedef FILETIME Time;
#else
    typedef time_t   Time;
#endif

    RT::PreviewRaytracer	_preview;	// Raytracer for preview
    RT::Scene *			_scene;		// Current scene loaded
    std::string			_file;		// Path to current scene file
    std::map<std::string, Time>	_time;		// Update time of associated file


  private:
    bool  			updateFiles();				// Check if current file and dependencies have been modified
    bool  			updateFile(std::string const &);	// Update modification time of file

  public:
    ControlState(std::string const &);
    ~ControlState();

    bool  			update(sf::Time) override;	// Wait for input
    void  			draw() override;		// Display raytracer image
  };
};

#endif