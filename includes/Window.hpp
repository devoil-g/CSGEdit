#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <map>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#ifdef _WIN32
#include <shobjidl.h>
#include <windows.h>
#endif

namespace RT
{
  namespace Config
  {
    namespace Window
    {
      std::string const		Title("CSGEdit");
      std::string const		Icon("C:/project/CSG-Raytracer/assets/icons/icon16.png");
      unsigned int const	Width(640);
      unsigned int const	Height(360);
    };
  };

  class Window
  {
  private:
    struct Mouse
    {
      int	x, y;			// Cursor position
      bool	left, middle, right;	// Buttons pressed
    };

    sf::RenderWindow			_window;		// SFML window
    RT::Window::Mouse			_mouse;			// Mouse position
    std::map<sf::Keyboard::Key, bool>	_pressed, _released;	// Maps of pressed/released key
    bool				_focus;			// True if focus on window

#ifdef _WIN32
    ITaskbarList3 *			_taskbar;		// Windows 7+ window handler
#else
    void *				_taskbar;
#endif

    Window();
    ~Window();

  public:
    static RT::Window &	Instance();			// Get instance (singleton)

    sf::RenderWindow &	window();			// Get SFML window
    bool		update();			// Update window (get events)
    void		display();			// Display rendered image
    void		draw(sf::Image const &);	// Draw an image centered / fitted in the window.

#ifdef _WIN32
    enum WindowFlag
    {
      NoProgress = TBPF_NOPROGRESS,		// Default taskbar icon
      Indeterminate = TBPF_INDETERMINATE,	// Green flickering icon
      Normal = TBPF_NORMAL,			// Green progress bar
      Error = TBPF_ERROR,			// Red progress bar
      Paused = TBPF_PAUSED			// Orange progress bar
    };
#else
    enum WindowFlag
    {
      NoProgress,
      Indeterminate,
      Normal,
      Error,
      Paused
    };
#endif

    void			setTaskbar(WindowFlag);			// Set taskbar status (Windows 7+ only)
    void			setTaskbar(WindowFlag, double);		// Set taskbar progress (Windows 7+ only)

    RT::Window::Mouse const &	mouse() const;				// Get mouse position/buttons
    bool			focus() const;				// Check if window has focus
    bool			key(sf::Keyboard::Key) const;		// Check if a key is currently pressed
    bool			keyPressed(sf::Keyboard::Key) const;	// Check if a key has been pressed
    bool			keyReleased(sf::Keyboard::Key) const;	// Check if a key has been released
  };
};

#endif
