#include "AbstractRaytracer.hpp"

RT::AbstractRaytracer::AbstractRaytracer()
  : _lock(), _thread(nullptr), _active(false)
{}

RT::AbstractRaytracer::~AbstractRaytracer()
{
  stop();
}

bool	RT::AbstractRaytracer::active() const
{
  return _active;
}

void	    RT::AbstractRaytracer::start()
{
  _lock.lock();

  // Stop running threads
  stop();
  
  // Start new rendering thread
  _active = true;
  _thread = new std::thread(&RT::AbstractRaytracer::begin, this);

  _lock.unlock();
}

void	    RT::AbstractRaytracer::stop()
{
  _lock.lock();

  // Ask running threads to stop
  _active = false;

  // Wait for running threads to terminate
  if (_thread != nullptr)
    _thread->join();
  delete _thread;
  _thread = nullptr;

  _lock.unlock();
}
