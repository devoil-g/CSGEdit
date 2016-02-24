#include <iostream>

#include "RenderPauseState.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"
#include "Config.hpp"

RT::RenderPauseState::RenderPauseState(RT::RenderRaytracer & render, RT::Scene * scene)
  : _image(), _render(render)
{
  _render.stop();
  RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Paused, _render.progress());

  // Apply a dark filter to image
  _image.create(scene->image().getSize().x, scene->image().getSize().y);
  for (unsigned int x = 0; x < _image.getSize().x; x++)
    for (unsigned int y = 0; y < _image.getSize().y; y++)
      _image.setPixel(x, y, (RT::Color(scene->image().getPixel(x, y)) / 2.f).sfml());
  
  // Prompt progress
  std::cout << "[Render] Paused at " << (int)(_render.progress() * 100.f) << "." << ((int)(_render.progress() * 1000.f)) % 10 << " %.                \r" << std::flush;
}

RT::RenderPauseState::~RenderPauseState()
{}

bool  RT::RenderPauseState::update(sf::Time)
{
  // Stop rendering, getting back to control state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Escape))
  {
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Error, _render.progress());
    RT::StateMachine::Instance().pop();
    RT::StateMachine::Instance().pop();
    return false;
  }

  // Resume rendering, getting back to render state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::P))
  {
    _render.start();
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Normal, _render.progress());
    RT::StateMachine::Instance().pop();
    return false;
  }

  return false;
}

void  RT::RenderPauseState::draw()
{
  RT::Window::Instance().draw(_image);
}