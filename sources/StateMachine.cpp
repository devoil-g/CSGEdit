#include <stdexcept>

#include "Color.hpp"
#include "StateMachine.hpp"
#include "Window.hpp"

RT::StateMachine::StateMachine()
{
  // Push default state
  push(new RT::StateMachine::DefaultState());
}

RT::StateMachine::~StateMachine()
{
  while (!_states.empty())
  {
    delete _states.top();
    _states.pop();
  }
}

RT::StateMachine &	RT::StateMachine::Instance()
{
  static StateMachine	singleton;

  // Return unique instance (singleton)
  return singleton;
}

void			RT::StateMachine::run()
{
  sf::Clock		clock;

  while (RT::Window::Instance().window().isOpen())
  {
    // Return if update return true
    if (RT::Window::Instance().update() || _states.top()->update(clock.restart()))
      return;

    // Draw image
    RT::Window::Instance().window().clear(RT::Color(0.084f).sfml());
    _states.top()->draw();

    // Display image
    RT::Window::Instance().display();
  }
}

void			RT::StateMachine::swap(RT::AbstractState * state)
{
  _lock.lock();

  // Cancel if nullptr
  if (state)
  {
    pop();
    push(state);
  }
  else
  {
    _lock.unlock();
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
  }

  _lock.unlock();
}

void			RT::StateMachine::push(RT::AbstractState * state)
{
  _lock.lock();

  // Cancel if nullptr
  if (state)
    _states.push(state);
  else
  {
    _lock.unlock();
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
  }

  _lock.unlock();
}

void			RT::StateMachine::pop()
{
  _lock.lock();

  // Pop state if possible
  if (_states.size() > 1)
  {
    delete _states.top();
    _states.pop();
  }
  else
  {
    _lock.unlock();
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
  }

  _lock.unlock();
}

bool			RT::StateMachine::empty() const
{
  return _states.size() == 1;
}

RT::StateMachine::DefaultState::DefaultState()
{}

RT::StateMachine::DefaultState::~DefaultState()
{}

bool			RT::StateMachine::DefaultState::update(sf::Time)
{
  // Always return true
  return true;
}

void			RT::StateMachine::DefaultState::draw()
{}
