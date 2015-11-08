#include <iostream>

#include "RenderState.hpp"
#include "RenderPauseState.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"
#include "Config.hpp"

RT::RenderState::RenderState(RT::Raytracer * raytracer)
  : _raytracer(raytracer), _wait(RT::Config::ThreadSleep)
{
  // Initialize and start final rendering
  _raytracer->render();
  _texture.loadFromImage(_raytracer->image());
  _sprite.setTexture(_texture);
  _raytracer->start();

  RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Normal, 0.f);
}

RT::RenderState::~RenderState()
{}

bool  RT::RenderState::update(sf::Time elapsed)
{
  // Get progress
  double  progress = _raytracer->progress();
  _elapsed += elapsed;

  // If rendering completed, stop, save image and pop state, getting back to control state
  if (progress == 1.f)
  {
    std::cout << "[Render] Completed in " << (int)_elapsed.asSeconds() / 3600 << "h " << (int)_elapsed.asSeconds() % 3600 / 60 << "m " << (int)_elapsed.asSeconds() % 60 << "s.        " << std::endl;
    _raytracer->stop();
    _raytracer->image().saveToFile("screenshots/screenshot_" + std::to_string(std::time(nullptr)) + ".png");
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Normal, 1.f);
    RT::StateMachine::Instance().pop();
    return false;
  }

  // If ESC, interrupt rendering, getting back to control state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Escape))
  {
    std::cout << "[Render] Interrupted at " << (int)(progress * 100.f) << "." << ((int)(progress * 1000.f)) % 10 << " % (" << (int)_elapsed.asSeconds() / 3600 << "h " << (int)_elapsed.asSeconds() % 3600 / 60 << "m " << (int)_elapsed.asSeconds() % 60 << "s elapsed).    " << std::endl;
    _raytracer->stop();
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Error, progress);
    RT::StateMachine::Instance().pop();
    return false;
  }

  // Get to pause state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::P))
  {
    RT::StateMachine::Instance().push(new RT::RenderPauseState(_raytracer));
    return false;
  }

  // Process wait timer
  if (_wait <= 0)
    _wait = RT::Config::ThreadSleep;
  _wait -= elapsed.asSeconds();

  // If timer over, update display
  if (_wait <= 0)
  {
    unsigned int  remaining = _elapsed.asSeconds() / progress * (1.f - progress);
    std::cout << "[Render] " << (int)(progress * 100.f) << "." << ((int)(progress * 1000.f)) % 10 << " % (" << remaining / 3600 << "h " << remaining % 3600 / 60 << "m " << remaining % 60 << "s remaining).    \r" << std::flush;
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Normal, progress);
    _texture.loadFromImage(_raytracer->image());
    _sprite.setTexture(_texture);
  }

  return false;
}

void  RT::RenderState::draw()
{
  RT::Window::Instance().window().draw(_sprite);
}