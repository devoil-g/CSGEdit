#include <iostream>

#include "ControlState.hpp"
#include "RenderState.hpp"
#include "StateMachine.hpp"
#include "Exception.hpp"
#include "Window.hpp"

RT::ControlState::ControlState(std::string const & file)
  : _raytracer(new RT::Raytracer()), _file(file)
{
  // Initialize file time
#ifdef WIN32
  _time.dwLowDateTime = 0;
  _time.dwHighDateTime = 0;
#else
  _time = 0;
#endif
  updateTime();

  // Load file
  _raytracer->load(file);
}

RT::ControlState::~ControlState()
{
  delete _raytracer;
}

bool  RT::ControlState::updateTime()
{
#ifdef WIN32
  HANDLE f = CreateFileA(_file.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

  if (f == NULL)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  
  FILETIME creation, lastaccess, lastwrite;
  GetFileTime(f, &creation, &lastaccess, &lastwrite);
  CloseHandle(f);

  if (lastwrite.dwHighDateTime > _time.dwHighDateTime || (lastwrite.dwHighDateTime == _time.dwHighDateTime && lastwrite.dwLowDateTime > _time.dwLowDateTime))
  {
    _time = lastwrite;
    return true;
  }
#else
  struct stat st;
  int fh = open(_file.c_str(), O_RDONLY);

  if (fh < 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
  
  fstat(fh, &st);
  close(fh);

  if (st.st_mtime > _time)
  {
    _time = st.st_mtime;
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
  if (updateTime())
  {
    _raytracer->load(_file);
    _raytracer->preview();
    _raytracer->start();
    return false;
  }

  // Save image
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::S))
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
      _raytracer->image().saveToFile(std::string(path));
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
      _time.dwLowDateTime = 0;
      _time.dwHighDateTime = 0;
    }
    else
      std::cerr << "[WorldEdit] Failed opening file." << std::endl;
#endif
  }

  // Launch render state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Return))
  {
    RT::StateMachine::Instance().push(new RT::RenderState(_raytracer));
    return false;
  }

  // Launch preview state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::P))
  {
    _raytracer->preview();
    _raytracer->start();
    return false;
  }

  return false;
}

void  RT::ControlState::draw()
{
  sf::Texture texture;
  sf::Sprite  sprite;

  // Get and display image from raytracer
  texture.loadFromImage(_raytracer->image());
  sprite.setTexture(texture);
  RT::Window::Instance().window().draw(sprite);
}