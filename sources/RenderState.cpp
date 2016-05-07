#include <exception>
#include <iostream>

#include "RenderPauseState.hpp"
#include "RenderState.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"

RT::RenderState::RenderState(RT::Scene * scene)
  : _elapsed(), _render(), _scene(scene), _wait(RT::Config::RenderState::Refresh)
{
  if (_scene == nullptr)
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  _render.load(scene);
  _render.start();

  RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Normal, 0.f);
}

RT::RenderState::~RenderState()
{
  // Reset scene HUD
  for (unsigned x = 0; x < _scene->hud().getSize().x; x++)
    for (unsigned y = 0; y < _scene->hud().getSize().y; y++)
      _scene->hud().setPixel(x, y, RT::Color(0.f).sfml(0.f));
}

bool  RT::RenderState::update(sf::Time elapsed)
{
  // Get progress
  double  progress = _render.progress();
  _elapsed += elapsed;

  // If rendering completed, stop, save image and pop state, getting back to control state
  if (progress == 1.f)
  {
    _render.stop();
    std::cout << "[Render] Completed in " << (int)_elapsed.asSeconds() / 3600 << "h " << (int)_elapsed.asSeconds() % 3600 / 60 << "m " << (int)_elapsed.asSeconds() % 60 << "s.        " << std::endl;

    // Save image
    _scene->image().saveToFile(RT::Config::ExecutablePath + "screenshot_" + std::to_string(std::time(nullptr)) + ".png");

    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Normal, 1.f);
    RT::StateMachine::Instance().pop();
    return false;
  }

  // If ESC, interrupt rendering, getting back to control state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Escape))
  {
    std::cout << "[Render] Interrupted at " << (int)(progress * 100.f) << "." << ((int)(progress * 1000.f)) % 10 << " % (" << (int)_elapsed.asSeconds() / 3600 << "h " << (int)_elapsed.asSeconds() % 3600 / 60 << "m " << (int)_elapsed.asSeconds() % 60 << "s elapsed).    " << std::endl;
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Error, progress);
    RT::StateMachine::Instance().pop();
    return false;
  }

  // Get to pause state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::P))
  {
    RT::StateMachine::Instance().push(new RT::RenderPauseState(_render, _scene, _elapsed));
    return false;
  }

  // Process wait timer
  while (_wait <= 0.f)
    _wait += RT::Config::RenderState::Refresh;
  _wait -= elapsed.asSeconds();

  // If timer over, update display
  if (_wait <= 0.f)
  {
    unsigned int  remaining = (unsigned int)(_elapsed.asSeconds() / progress * (1.f - progress));
    std::cout << "[Render] " << (int)(progress * 100.f) << "." << ((int)(progress * 1000.f)) % 10 << " % (" << remaining / 3600 << "h " << remaining % 3600 / 60 << "m " << remaining % 60 << "s remaining).    \r" << std::flush;
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Normal, progress);
  }

  return false;
}

void  RT::RenderState::draw()
{
  RT::Window::Instance().draw(_scene->image());
  RT::Window::Instance().draw(_scene->hud());
}
