#ifndef _CONTROL_STATE_HPP_
#define _CONTROL_STATE_HPP_

#ifdef WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

#include <string>

#include "AbstractState.hpp"
#include "Raytracer.hpp"

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

    RT::Raytracer * const	_raytracer; // Pointer to raytracer
    std::string			_file;
    t_FileTime			_time;

  private:
    bool  updateTime();

  public:
    ControlState(std::string const &);
    ~ControlState();

    bool  update(sf::Time) override;  // Wait for input
    void  draw() override;	      // Display raytracer image
  };
};

#endif