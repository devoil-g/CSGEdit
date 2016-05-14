#ifndef _ABSTRACT_RAYTRACER_HPP_
#define _ABSTRACT_RAYTRACER_HPP_

#include <mutex>
#include <thread>

#include "Scene.hpp"

namespace RT
{
  namespace Config
  {
    namespace Raytracer
    {
      unsigned int const	BlockSize(16);		// Size of a block of pixel rendered by a thread
      unsigned int const	MaxRecursivite(3);	// Maximum of 'bounce' on a mirror
    };
  };

  class AbstractRaytracer
  {
  private:
    std::recursive_mutex	_lock;			// Lock for start & stop
    std::thread *		_thread;		// Main rendering threads
    bool			_active;		// True to continue rendering, false to stop

    virtual void		begin() = 0;		// Method called once when start()

  protected:
    bool			active() const;		// Return active status

  public:
    AbstractRaytracer();
    virtual ~AbstractRaytracer();

    void			start();		// Start rendering threads
    void			stop();			// Stop rendering threads

    virtual void		load(RT::Scene *) = 0;	// Load a new scene
    virtual double		progress() const = 0;	// Return current progress (0-1)
  };
};

#endif
