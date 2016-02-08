#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <list>
#include <SFML/Graphics/Image.hpp>
#include <string>

#include "AbstractLight.hpp"
#include "AbstractTree.hpp"
#include "Matrix.hpp"

namespace RT
{
  class Scene
  {
  public:
    Scene();
    ~Scene();

    sf::Image					image;		// Rendered image of scene
    Math::Matrix<4, 4>				camera;		// Matrix camera
    RT::AbstractTree const *			tree;		// CSG tree
    std::list<RT::AbstractLight const *>	light;		// List of light
    std::string					file;		// Master file of scene
    std::list<std::string>			dependencies;	// List of dependencies of scene
  };
};

#endif