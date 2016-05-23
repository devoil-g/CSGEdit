#include "AbstractRenderer.hpp"

RT::AbstractRenderer::AbstractRenderer()
  : _lock(), _thread(nullptr), _active(false)
{}

RT::AbstractRenderer::~AbstractRenderer()
{
  stop();
}

bool	RT::AbstractRenderer::active() const
{
  return _active;
}

void	RT::AbstractRenderer::start()
{
  _lock.lock();

  // Stop running threads
  stop();

  // Start new rendering thread
  _active = true;
  _thread = new std::thread(&RT::AbstractRenderer::begin, this);

  _lock.unlock();
}

void	RT::AbstractRenderer::stop()
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
