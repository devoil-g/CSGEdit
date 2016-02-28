#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

#include "Config.hpp"
#include "ControlState.hpp"
#include "Exception.hpp"
#include "Parser.hpp"
#include "PreviewRaytracer.hpp"
#include "RenderRaytracer.hpp"
#include "RenderState.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"

RT::ControlState::ControlState(std::string const & file)
  : _preview(), _scene(nullptr), _file(file), _time(), _camera(Math::Matrix<4, 4>::identite())
{}

RT::ControlState::~ControlState()
{
  _preview.stop();
  delete _scene;
}

bool	RT::ControlState::updateFiles()
{
  bool	result = updateFile(_file);

  if (_scene != nullptr)
    for (std::list<std::string>::const_iterator it = _scene->dependencies().begin(); it != _scene->dependencies().end(); it++)
      result |= updateFile(*it);

  return result;
}

bool	RT::ControlState::updateFile(std::string const & file)
{
#ifdef WIN32
  FILETIME  creation, lastaccess, lastwrite;
  HANDLE    f = CreateFileA(file.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

  if (f == NULL)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  
  GetFileTime(f, &creation, &lastaccess, &lastwrite);
  CloseHandle(f);

  if (lastwrite.dwHighDateTime > _time[file].dwHighDateTime || (lastwrite.dwHighDateTime == _time[file].dwHighDateTime && lastwrite.dwLowDateTime > _time[file].dwLowDateTime))
  {
    _time[file] = lastwrite;
    return true;
  }
#else
  struct stat	st;
  
  if (stat(_file.c_str(), &st) < 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  
  if (st.st_mtime > _time[file])
  {
    _time[file] = st.st_mtime;
    return true;
  }
#endif

  return false;
}

bool	RT::ControlState::update(sf::Time)
{
  // Quit if ESC
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Escape))
    return true;

  // Update file time, preview if change detected
  if (updateFiles() || RT::Window::Instance().keyPressed(sf::Keyboard::Key::R))
  {
    RT::Parser	parser;

    if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::R))
      _camera = Math::Matrix<4, 4>::identite();

    // Reload scene
    _preview.stop();
    delete _scene;
    _scene = parser.load(_file);
    updateFiles();

    // Set/get camera position
    if (_scene && _camera == Math::Matrix<4, 4>::identite())
      _camera = _scene->camera();
    else
      _scene->camera() = _camera;

    // Start preview rendering
    _preview.load(_scene);
    _preview.start();
    
    return false;
  }

  // Move camera
  if (_scene && RT::Window::Instance().focus())
  {
    // Camera rotation X
    if (RT::Window::Instance().mouse().middle || (RT::Window::Instance().mouse().left == true && RT::Window::Instance().mouse().right == true))
      _camera *= Math::Matrix<4, 4>::rotation((double)RT::Window::Instance().mouse().rx / 2.f, 0.f, 0.f);
    // Camera rotation YZ
    if (RT::Window::Instance().mouse().left == true && RT::Window::Instance().mouse().right == false)
      _camera *= Math::Matrix<4, 4>::rotation(0.f, (double)RT::Window::Instance().mouse().ry / 2.f, -(double)RT::Window::Instance().mouse().rx / 2.f);
    // Camera lateral & vertical translation
    if (RT::Window::Instance().mouse().left == false && RT::Window::Instance().mouse().right == true)
      _camera *= Math::Matrix<4, 4>::translation(0.f, (double)RT::Window::Instance().mouse().rx, (double)RT::Window::Instance().mouse().ry);
    // Camera forward & backward translation
    if (RT::Window::Instance().key(sf::Keyboard::LShift) == false)
      _camera *= Math::Matrix<4, 4>::translation((double)RT::Window::Instance().mouse().wheel * 10.f, 0.f, 0.f);
    // Camera zoom & un-zoom
    if (RT::Window::Instance().key(sf::Keyboard::LShift) == true)
      _camera *= Math::Matrix<4, 4>::scale(std::pow((double)RT::Window::Instance().mouse().wheel > 0 ? 1.1f : 0.9f, std::abs((double)RT::Window::Instance().mouse().wheel)), 1.f, 1.f);

    // Update camera if position changed
    if (_camera != _scene->camera())
    {
      _preview.stop();
      _scene->camera() = _camera;
      _preview.load(_scene);
      _preview.start();
    }
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
      _scene->image().saveToFile(std::string(path));
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

    if (GetOpenFileName(&fileinfo))
    {
      _file = std::string(path);
      _time.clear();
      std::cout << "[" << RT::Config::Window::Title << "] Opening file '" << _file << "'." << std::endl;
    }
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

void	RT::ControlState::draw()
{
  if (_scene)
    RT::Window::Instance().draw(_scene->image());
}