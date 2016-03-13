#ifndef _SCENE_LIBRARY_HPP_
#define _SCENE_LIBRARY_HPP_

#include <map>
#include <string>
#include <utility>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include "Scene.hpp"

namespace RT
{
  class SceneLibrary
  {
  public:
#ifdef _WIN32
    typedef FILETIME	Time;	// Time structure (Windows, contain creation, last access & last write data)
#else
    typedef time_t	Time;	// Time structure (linux, time since Epoch in seconds)
#endif

  private:
    std::map<std::string, std::pair<Time, RT::Scene>>	_library;	// Cache of scene loaded
    
    Time	time(std::string const &) const;	// Get time of file
    void	load(std::string const &);		// Load/reload a scene

    SceneLibrary();
    ~SceneLibrary();

  public:
    static RT::SceneLibrary &	Instance();	// Get instance (singleton)

    RT::Scene *	get(std::string const &);	// Get a scene
    bool	change() const;			// Return true if change in loaded files
    void	update();			// Reload modified files
    void	clear();			// Clear cache
  };
};

#endif
