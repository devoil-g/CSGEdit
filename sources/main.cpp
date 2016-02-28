#include <iostream>
#include <string>

#include "Config.hpp"
#include "ControlState.hpp"
#include "Exception.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
// For thread initialization
#include <X11/Xlib.h>
#endif

namespace RT
{
  void	initialize(int argc, char ** argv)
  {
    RT::Config::initialize(argc, argv);
    RT::Material::initialize();
    Math::initialize();
  }
};

int	main(int argc, char ** argv)
{
  std::string file;

#ifdef _WIN32
  // See MSDN of GetOpenFileName
  OPENFILENAME  fileinfo;
  CHAR		path[MAX_PATH];
    
  memset(path, 0, sizeof(path));
  fileinfo.lStructSize = sizeof(OPENFILENAME);
  fileinfo.hwndOwner = nullptr;
  fileinfo.lpstrFilter = "All supported format (.scn)\0*.scn\0Scene description (.scn)\0*.scn\0All files\0*\0\0";
  fileinfo.lpstrCustomFilter = nullptr;
  fileinfo.nFilterIndex = 3;
  fileinfo.lpstrFile = path;
  fileinfo.nMaxFile = MAX_PATH;
  fileinfo.lpstrFileTitle = nullptr;
  fileinfo.lpstrInitialDir = nullptr;
  fileinfo.lpstrTitle = nullptr;
  fileinfo.Flags = OFN_FILEMUSTEXIST;
  fileinfo.lpstrDefExt = nullptr;
  fileinfo.FlagsEx = 0;
  
  // Remove false to enable OpenFileDialogWindow
  if (GetOpenFileName(&fileinfo))
    file = std::string(path);
  else
  {
    std::cerr << "Error: failed opening file." << std::endl;
    return EXIT_FAILURE;
  }
#else
  if (argc == 2)
  {
    file = std::string(argv[1]);

    struct stat	file_stat;
    if (stat(file.c_str(), &file_stat) == -1)
    {
      std::cerr << "Error: " << std::string(strerror(errno)) << "." << std::endl;
      return EXIT_FAILURE;
    }
    if (!S_ISREG(file_stat.st_mode))
    {
      std::cerr << "Error: '" << std::string(argv[1]) << "' is not a regular file." << std::endl;
      return EXIT_FAILURE;
    }
  }
  else
  {
    std::cerr << "Usage: " << std::string(argv[0]) << " [FILE]" << std::endl;
    return EXIT_FAILURE;
  }
#endif
  
#ifdef __linux__
  XInitThreads();
#endif

  try
  {
    std::cout << "[" << RT::Config::Window::Title << "] Opening file '" << file << "'." << std::endl;
    RT::initialize(argc, argv);

    // Push initial state in state machine here
    RT::StateMachine::Instance().push(new RT::ControlState(file));
    RT::StateMachine::Instance().run();
  }
  catch (RT::Exception exception)
  {
#ifdef _WIN32
    MessageBox(
      RT::Window::Instance().window().getSystemHandle(),
      exception.what(),
      "Error - Exception thrown",
      MB_OK | MB_ICONSTOP
      );
#else
    std::cerr << "[Exception]: " << exception.what() << std::endl;
#endif
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
