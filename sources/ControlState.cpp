#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

#include "Config.hpp"
#include "ControlState.hpp"
#include "RenderState.hpp"
#include "StateMachine.hpp"
#include "Exception.hpp"
#include "Parser.hpp"
#include "PreviewRaytracer.hpp"
#include "RenderRaytracer.hpp"
#include "Window.hpp"

RT::ControlState::ControlState(std::string const & file)
  : _preview(), _scene(nullptr), _file(file), _time()
{}

RT::ControlState::~ControlState()
{
  _preview.stop();
  delete _scene;
}

bool  RT::ControlState::updateFiles()
{
  bool	result = updateFile(_file);

  if (_scene != nullptr)
    for (std::list<std::string>::const_iterator it = _scene->dependencies.begin(); it != _scene->dependencies.end(); it++)
      result |= updateFile(*it);

  return result;
}

bool  RT::ControlState::updateFile(std::string const & file)
{
#ifdef WIN32
  HANDLE f = CreateFileA(file.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

  if (f == NULL)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  
  FILETIME creation, lastaccess, lastwrite;
  GetFileTime(f, &creation, &lastaccess, &lastwrite);
  CloseHandle(f);

  if (lastwrite.dwHighDateTime > _time[file].dwHighDateTime || (lastwrite.dwHighDateTime == _time[file].dwHighDateTime && lastwrite.dwLowDateTime > _time[file].dwLowDateTime))
  {
    _time[file] = lastwrite;
    return true;
  }
#else
  struct stat st;
  int fh = open(_file.c_str(), O_RDONLY);

  if (fh < 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  
  fstat(fh, &st);
  close(fh);

  if (st.st_mtime > _time[file])
  {
    _time[file] = st.st_mtime;
    return true;
  }
#endif

  return false;
}

bool  RT::ControlState::update(sf::Time)
{
  // Quit if ESC
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Escape))
    return true;

  // Update file time, preview if change detected
  if (updateFiles())
  {
    RT::Parser	parser;

    // Reload scene
    _preview.stop();
    delete _scene;
    _scene = parser.load(_file);
    _preview.load(_scene);
    if (_scene)
      _preview.start();
    
    return false;
  }

  // Save image
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::S) && _scene)
  {
#ifdef _WIN32
    // See MSDN of GetOpenFileName
    OPENFILENAME  fileinfo;
    CHAR	  path[MAX_PATH];

    memset(path, 0, sizeof(path));
    fileinfo.lStructSize = sizeof(OPENFILENAME);
    fileinfo.hwndOwner = nullptr;
    fileinfo.lpstrFilter = "Portable Network Graphics (.png)\0*.png\0BitMap (.bmp)\0*.bmp\0Truevision Targa (.tga)\0*.tga\0Joint Photographic Experts Group (.jpg)\0*.jpg\0All supported formats\0*png;*.bmp;*.tga;*.jpg\0\0";
    fileinfo.lpstrCustomFilter = nullptr;
    fileinfo.nFilterIndex = 5;
    fileinfo.lpstrFile = path;
    fileinfo.nMaxFile = MAX_PATH;
    fileinfo.lpstrFileTitle = nullptr;
    fileinfo.lpstrInitialDir = nullptr;
    fileinfo.lpstrTitle = nullptr;
    fileinfo.Flags = 0;
    fileinfo.lpstrDefExt = ".png";
    fileinfo.FlagsEx = 0;

    if (GetSaveFileName(&fileinfo))
      _scene->image.saveToFile(std::string(path));
#endif
  }

  // Open file
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::O))
  {
#ifdef _WIN32
    // See MSDN of GetOpenFileName
    OPENFILENAME	fileinfo;
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
    {
      _file = std::string(path);
      _time.clear();
      std::cout << "[" << RT::Config::WindowTitle << "] Opening file '" << _file << "'." << std::endl;
    }
    else
      std::cout << "[" << RT::Config::WindowTitle << "] Failed opening file." << std::endl;
#endif
  }

  // Launch render state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Return) && _scene)
  {
    _preview.stop();
    RT::StateMachine::Instance().push(new RT::RenderState(_scene));
    return false;
  }

  // Launch preview state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::P))
  {
    _preview.load(_scene);
    _preview.start();
    return false;
  }

  return false;
}

void  RT::ControlState::draw()
{
  if (_scene)
    RT::Window::Instance().draw(_scene->image);
}