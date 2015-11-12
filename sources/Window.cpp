#include "Window.hpp"
#include "Exception.hpp"
#include "Config.hpp"

RT::Window::Window()
  : _window(sf::VideoMode(RT::Config::WindowWidth, RT::Config::WindowHeight), std::string(RT::Config::WindowTitle), sf::Style::Titlebar | sf::Style::Close), _focus(true)
{
  // Activate V-sync (limit fps)
  _window.setVerticalSyncEnabled(true);

  // Disabled key repeate
  _window.setKeyRepeatEnabled(false);

  // Set window icon
  sf::Image icon;
  if (icon.loadFromFile(RT::Config::WindowIcon))
    _window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  // Hide cursor if mouse enabled
  if (RT::Config::MouseEnabled)
    _window.setMouseCursorVisible(false);
  else
    _window.setMouseCursorVisible(true);

#ifdef _WIN32
  CoInitialize(nullptr);
  CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskbarList3, (void **)&_taskbar);
  if (_taskbar == nullptr)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif
}

RT::Window::~Window()
{}

RT::Window &  RT::Window::Instance()
{
  static RT::Window singleton;

  // Return unique instance (singleton)
  return singleton;
}

sf::RenderWindow &  RT::Window::window()
{
  return _window;
}

bool  RT::Window::update()
{
  sf::Event event;

  // Clear pressed/released key maps
  _pressed.clear();
  _released.clear();

  while (_window.pollEvent(event))
  {
    // Stop if window closed
    if (event.type == sf::Event::Closed)
      return true;

    // Show/hide cursor if gained/lost focus
    if (event.type == sf::Event::GainedFocus)
    {
      if (RT::Config::MouseEnabled)
	_window.setMouseCursorVisible(false);
      _focus = true;
    }
    if (event.type == sf::Event::LostFocus)
    {
      if (RT::Config::MouseEnabled)
	_window.setMouseCursorVisible(true);
      _focus = false;
    }

    // Set pressed/released key
    if (event.type == sf::Event::KeyPressed)
      _pressed[event.key.code] = true;
    if (event.type == sf::Event::KeyReleased)
      _released[event.key.code] = true;
  }

  // If mouse enabled, get mouse position/buttons status, and recenter cursor
  if (RT::Config::MouseEnabled && _focus)
  {
    _mouse.x = sf::Mouse::getPosition(_window).x - _window.getSize().x / 2;
    _mouse.y = sf::Mouse::getPosition(_window).y - _window.getSize().y / 2;
    _mouse.left = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
    _mouse.right = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
    _mouse.middle = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);

    sf::Mouse::setPosition(sf::Vector2i(_window.getSize().x / 2, _window.getSize().y / 2), _window);
  }
  else
  {
    _mouse.x = 0;
    _mouse.y = 0;
    _mouse.left = false;
    _mouse.right = false;
    _mouse.middle = false;
  }

  return false;
}

void  RT::Window::display()
{
  _window.display();
}

void  RT::Window::setTaskbar(RT::Window::WindowFlag flag)
{
#ifdef _WIN32
  _taskbar->SetProgressState(_window.getSystemHandle(), (TBPFLAG)flag);
#endif
}

void  RT::Window::setTaskbar(RT::Window::WindowFlag flag, double progress)
{
  // Apply flag
  setTaskbar(flag);

  // Check for progress value
  if (progress < 0.f)
    progress = 0.f;
  if (progress > 1.f)
    progress = 1.f;

#ifdef _WIN32
  _taskbar->SetProgressValue(_window.getSystemHandle(), (ULONGLONG)(progress * 1000), 1000);
#endif
}

RT::Window::Mouse const & RT::Window::mouse() const
{
  return _mouse;
}

bool  RT::Window::focus() const
{
  return _focus;
}

bool  RT::Window::key(sf::Keyboard::Key key) const
{
  // Check key status only if window has focus
  if (focus())
    return sf::Keyboard::isKeyPressed(key);
  else
    return false;
}

bool  RT::Window::keyPressed(sf::Keyboard::Key key) const
{
  // Check key status only if window has focus
  if (focus() && _pressed.find(key) != _pressed.end())
    return true;
  else
    return false;
}

bool  RT::Window::keyReleased(sf::Keyboard::Key key) const
{
  // Check key status only if window has focus
  if (focus() && _released.find(key) != _released.end())
    return true;
  else
    return false;
}