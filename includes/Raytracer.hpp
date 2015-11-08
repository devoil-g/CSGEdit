#ifndef _RAYTRACER_HPP_
#define _RAYTRACER_HPP_

#include <string>
#include <vector>
#include <list>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Sleep.hpp>

#include "AbstractLight.hpp"
#include "AbstractTree.hpp"
#include "Color.hpp"
#include "Matrix.hpp"

namespace RT
{
  class Raytracer
  {
  private:
    enum ProgressState
    {
      Wait,	// Not rendered
      Progress,	// Rendering in progress
      Done	// Rendered
    };

    sf::Image				  _image;	    // Image stocking result
    sf::Mutex				  _lock;	    // Lock for thread-safe operations
    RT::AbstractTree const *		  _tree;	    // CSG tree to render
    std::list <RT::AbstractLight const *> _light;	    // List of lights to render
    Math::Matrix<4, 4>			  _camera;	    // Transformation matrix of camera
    std::vector<ProgressState>		  _grid;	    // Grid of rendered zone (for multi-threaded rendering)
    std::vector<sf::Thread *>		  _thread;	    // List of active thread

    void (RT::Raytracer::*_method)(unsigned int);	    // Rendering method called by threads

    void	routine();				    // Thread routine, calling _method until rendering finished or interrupted
    void	preview(unsigned int);			    // Render a zone using preview engine
    RT::Color	preview(unsigned int, unsigned int) const;  // Render a pixel using preview engine
    void	render(unsigned int);			    // Render a zone
    RT::Color	render(unsigned int, unsigned int) const;   // Render a pixel

    RT::Color	renderAntialiasing(unsigned int, unsigned int) const;				      // Render anti-aliasing according to RT::Config::Antialiasing
    RT::Color	renderAnaglyph3D(Math::Ray const &) const;					      // Render anaglyph image using parameters stored in RT::Config::Anaglyph3D
    RT::Color	renderDephOfField(Math::Ray const &) const;					      // Render deph of field image using parameters stored in RT::Config::DephOfField
    RT::Color	renderRay(Math::Ray const &, unsigned int = 0) const;				      // Render a ray with CSG tree
    RT::Color	renderReflection(Math::Ray const &, RT::Intersection const &, unsigned int) const;    // Render reflection of ray  
    RT::Color	renderTransparency(Math::Ray const &, RT::Intersection const &, unsigned int) const;  // Render transparency
    RT::Color	renderLight(Math::Ray const &, RT::Intersection const &, unsigned int) const;	      // Render light

  public:
    Raytracer();
    ~Raytracer();

    bool			load(std::string const &);  // Load a file
    void			preview();		    // Initialize Raytracer to render a preview
    void			render();		    // Initialize Raytracer to render final image
    void	        	start();		    // Start rendering threads
    void			stop();			    // Stop rendering threads
    double			progress();		    // Return progress % (0-1)

    Math::Matrix<4,4 > const &	camera() const;		    // Return camera transformation matrix
    sf::Image const &		image() const;		    // Return a reference to the rendered image
  };
};

#endif
