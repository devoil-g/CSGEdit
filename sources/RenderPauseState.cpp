#include <iostream>

#include "RenderPauseState.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"
#include "Config.hpp"

RT::RenderPauseState::RenderPauseState(RT::Raytracer * raytracer)
  : _raytracer(raytracer)
{
  _raytracer->stop();
  RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Paused, _raytracer->progress());

  // Apply a dark filter to image
  _image = _raytracer->image();
  for (unsigned int x = 0; x < _image.getSize().x; x++)
    for (unsigned int y = 0; y < _image.getSize().y; y++)
    {
      sf::Color	clr = _image.getPixel(x, y);

      clr.r /= 2;
      clr.g /= 2;
      clr.b /= 2;
      _image.setPixel(x, y, clr);
    }
  _texture.loadFromImage(_image);
  _sprite.setTexture(_texture);

  // Prompt progress
  std::cout << "[Render] Paused at " << (int)(_raytracer->progress() * 100.f) << "." << ((int)(_raytracer->progress() * 1000.f)) % 10 << " %.                \r" << std::flush;
}

RT::RenderPauseState::~RenderPauseState()
{}

bool  RT::RenderPauseState::update(sf::Time)
{
  // Stop rendering, getting back to control state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Escape))
  {
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Error, _raytracer->progress());
    RT::StateMachine::Instance().pop();
    RT::StateMachine::Instance().pop();
    return false;
  }

  // Resume rendering, getting back to render state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::P))
  {
    _raytracer->start();
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Normal, _raytracer->progress());
    RT::StateMachine::Instance().pop();
    return false;
  }

  return false;
}

void  RT::RenderPauseState::draw()
{
  RT::Window::Instance().window().draw(_sprite);
}