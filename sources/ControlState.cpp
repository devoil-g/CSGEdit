#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

#include "Config.hpp"
#include "ControlState.hpp"
#include "Exception.hpp"
#include "PreviewRaytracer.hpp"
#include "RenderRaytracer.hpp"
#include "RenderState.hpp"
#include "SceneLibrary.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"

RT::ControlState::ControlState(std::string const & file)
  : _preview(), _scene(nullptr), _file(file), _camera()
{
  RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Indeterminate);

  // Load scene
  RT::SceneLibrary::Instance().clear();
  _scene = RT::SceneLibrary::Instance().get(file);
  _camera = _scene->camera();

  // Start preview rendering
  _preview.load(_scene);
  _preview.start();

  RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::NoProgress);
}

RT::ControlState::~ControlState()
{
  _preview.stop();
}

bool	RT::ControlState::update(sf::Time)
{
  // Quit if ESC
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Escape))
    return true;

  // Reload if file modified
  if (RT::SceneLibrary::Instance().change())
  {
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Indeterminate);

    // Stop rendering
    _preview.stop();

    // Update scene
    RT::SceneLibrary::Instance().update();

    // Get camera position
    _scene->camera() = _camera;

    // Start preview rendering
    _preview.load(_scene);
    _preview.start();

    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::NoProgress);

    return false;
  }

  // Force reload
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::R))
  {
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Indeterminate);

    // Stop rendering
    _preview.stop();

    // Reload scene
    RT::SceneLibrary::Instance().clear();
    _scene = RT::SceneLibrary::Instance().get(_file);

    // Get camera position
    _camera = _scene->camera();

    // Start preview rendering
    _preview.load(_scene);
    _preview.start();

    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::NoProgress);

    return false;
  }

  // Move camera
  if (RT::Window::Instance().focus())
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
      std::cout << "[" << RT::Config::Window::Title << "] Opening file '" << _file << "'." << std::endl;

      RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Indeterminate);

      // Load new scene
      _scene = RT::SceneLibrary::Instance().get(_file);
      _camera = _scene->camera();
      _preview.load(_scene);
      _preview.start();

      RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::NoProgress);
    }
#endif
  }

  // Launch render state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Return))
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
  RT::Window::Instance().draw(_scene->image());
  RT::Window::Instance().draw(_scene->hud());
}
