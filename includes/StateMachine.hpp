#ifndef _STATE_MACHINE_HPP_
#define _STATE_MACHINE_HPP_

#include <mutex>
#include <stack>

#include "AbstractState.hpp"

namespace RT
{
  class StateMachine
  {
  private:
    class DefaultState : public RT::AbstractState
    {
    public:
      DefaultState();
      ~DefaultState();

      bool	update(sf::Time) override;	// Force quit
      void	draw() override;		// Do nothing
    };

    std::stack<RT::AbstractState *> _states;	// Stack of states
    std::recursive_mutex	    _lock;	// Lock for multi-threading

    StateMachine();
    ~StateMachine();

  public:
    static RT::StateMachine &	Instance();	// Return unique instance (singleton)

    void	run();				// Run state machine

    void	swap(RT::AbstractState *);	// Change current state to another
    void	push(RT::AbstractState *);	// Push a state on the stack
    void	pop();				// Pop top state of the stack
    bool	empty() const;			// Test if remaining states (don't include DefaultState)
  };
};

#endif
