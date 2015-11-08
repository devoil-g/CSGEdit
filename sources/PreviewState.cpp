#include <iostream>

#include "PreviewState.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"
#include "Config.hpp"

RT::PreviewState::PreviewState(RT::Raytracer * raytracer)
  : _raytracer(raytracer), _wait(RT::Config::ThreadSleep)
{
  // Initialize and start preview rendering
  _raytracer->preview();
  _texture.loadFromImage(_raytracer->image());
  _sprite.setTexture(_texture);
  _raytracer->start();

  RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::NoProgress);
}

RT::PreviewState::~PreviewState()
{}

bool  RT::PreviewState::update(sf::Time elapsed)
{
  // Get progress
  double  progress = _raytracer->progress();
  _elapsed += elapsed;

  // If rendering completed, stop, save image and pop state, getting back to control state
  if (progress == 1.f)
  {
    std::cout << "[Preview] Completed in " << (int)_elapsed.asSeconds() / 3600 << "h " << (int)_elapsed.asSeconds() % 3600 / 60 << "m " << (int)_elapsed.asSeconds() % 60 << "s.        " << std::endl;
    _raytracer->stop();
    RT::StateMachine::Instance().pop();
    return false;
  }

  // If ESC, interrupt rendering, getting back to control state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Escape))
  {
    std::cout << "[Preview] Interrupted at " << (int)(progress * 100.f) << "." << ((int)(progress * 1000.f)) % 10 << " % (" << (int)_elapsed.asSeconds() / 3600 << "h " << (int)_elapsed.asSeconds() % 3600 / 60 << "m " << (int)_elapsed.asSeconds() % 60 << "s elapsed).    " << std::endl;
    _raytracer->stop();
    RT::StateMachine::Instance().pop();
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
    std::cout << "[Preview] " << (int)(progress * 100.f) << "." << ((int)(progress * 1000.f)) % 10 << " % (" << remaining / 3600 << "h " << remaining % 3600 / 60 << "m " << remaining % 60 << "s remaining).    \r" << std::flush;
    _texture.loadFromImage(_raytracer->image());
    _sprite.setTexture(_texture);
  }

  return false;
}

void  RT::PreviewState::draw()
{
  RT::Window::Instance().window().draw(_sprite);
}
