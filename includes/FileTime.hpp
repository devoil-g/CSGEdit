#ifndef _FILE_TIME_HPP_
#define _FILE_TIME_HPP_

#include <string>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace RT
{
  class FileTime
  {
  private:

#ifdef _WIN32
    typedef FILETIME	Time;		// Time structure (Windows, contain creation, last access & last write data)
#else
    typedef time_t	Time;		// Time structure (linux, time since Epoch in seconds)
#endif

    Time		_time;		// Last time status of file
    std::string		_file;		// File being watched

  public:
    FileTime();
    FileTime(std::string const &);
    ~FileTime();

    bool		update();	// Update file time, return true if changed
    std::string const &	file() const;	// Return file name
  };
};

#endif