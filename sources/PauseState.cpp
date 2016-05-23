#include <iostream>

#include "PauseState.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"

RT::PauseState::PauseState(RT::AbstractRenderer * renderer, RT::Scene * scene, sf::Time elapsed)
  : _image(), _renderer(renderer)
{
  _renderer->stop();
  RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Paused, _renderer->progress());

  // Apply a dark filter to image
  _image.create(scene->image().getSize().x, scene->image().getSize().y);
  for (unsigned int x = 0; x < _image.getSize().x; x++)
    for (unsigned int y = 0; y < _image.getSize().y; y++)
      _image.setPixel(x, y, (RT::Color(scene->image().getPixel(x, y)) / 2.f).sfml());

  // Prompt progress
  std::cout << "[Render] Paused at " << (int)(_renderer->progress() * 100.f) << "." << ((int)(_renderer->progress() * 1000.f)) % 10 << " % (" << (int)elapsed.asSeconds() / 3600 << "h " << (int)elapsed.asSeconds() % 3600 / 60 << "m " << (int)elapsed.asSeconds() % 60 << "s elapsed).        \r" << std::flush;
}

RT::PauseState::~PauseState()
{}

bool	RT::PauseState::update(sf::Time)
{
  // Stop rendering, getting back to control state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::Escape))
  {
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Error, _renderer->progress());
    RT::StateMachine::Instance().pop();
    RT::StateMachine::Instance().pop();
    return false;
  }

  // Resume rendering, getting back to render state
  if (RT::Window::Instance().keyPressed(sf::Keyboard::Key::P))
  {
    _renderer->start();
    RT::Window::Instance().setTaskbar(RT::Window::WindowFlag::Normal, _renderer->progress());
    RT::StateMachine::Instance().pop();
    return false;
  }

  return false;
}

void	RT::PauseState::draw()
{
  RT::Window::Instance().draw(_image);
}
