#include <iostream>
#include <thread>

#include "Config.hpp"
#include "Exception.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace RT
{
  namespace Config
  {
    std::string	  ExecutablePath(".");
    unsigned int  ThreadNumber(0);
  };
};

void  RT::Config::initialize(int argc, char **argv)
{
#ifdef _WIN32
  HMODULE	handle;
  WCHAR		path[MAX_PATH];
  std::wstring	result;

  // This error should never happen...
  handle = GetModuleHandle(nullptr);
  if (handle == nullptr)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  GetModuleFileNameW(handle, path, MAX_PATH);
  result = std::wstring(path).substr(0, std::wstring(path).find_last_of('\\'));

  RT::Config::ExecutablePath = std::string(result.begin(), result.end()) + std::string("\\");
#else
  // This error should never happen...
  if (argc < 1)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  if (std::string(argv[0]).find_last_of('/') == std::string::npos)
    RT::Config::ExecutablePath = std::string(".");
  else
    RT::Config::ExecutablePath = std::string(argv[0]).substr(0, std::string(argv[0]).find_last_of('/'));

  RT::Config::ExecutablePath += std::string("/");
#endif

  // Using C++11 to detect maximum of thread concurrency.
  unsigned int	systemThread = std::thread::hardware_concurrency();

  // If failed...
  if (systemThread == 0)
  {
    RT::Config::ThreadNumber = 1;
    std::cerr << "Error: failed to detect maximum of concurrency thread." << std::endl;
  }
  // If only 1 thread...
  else if (systemThread == 1)
    RT::Config::ThreadNumber = 1;
  // If +1 thread, keep one thread for display.
  else
    RT::Config::ThreadNumber = systemThread - 1;

  std::cout << "[Config] Number of rendering thread set to '" << RT::Config::ThreadNumber << "'." << std::endl;
}