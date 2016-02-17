#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <list>
#include <SFML/Graphics/Image.hpp>
#include <string>

#include "AbstractLight.hpp"
#include "AbstractTree.hpp"
#include "Color.hpp"
#include "Matrix.hpp"

namespace RT
{
  class Scene
  {
  private:
    struct Config
    {
      unsigned int				threadNumber;
      unsigned int				liveAntiAliasing;
      unsigned int				postAntiAliasing;
      RT::Color					lightAmbient;
      RT::Color					lightDiffuse;
      RT::Color					lightSpecular;
      double					dofAperture;
      double					dofFocal;
      unsigned int				dofQuality;
      double					anaglyphOffset;
      double					anaglyphFocal;
      RT::Color					anaglyphMaskLeft;
      RT::Color					anaglyphMaskRight;
    };

  public:
    Scene();
    ~Scene();

    sf::Image					image;		// Rendered image of scene
    Math::Matrix<4, 4>				camera;		// Matrix camera
    RT::AbstractTree const *			tree;		// CSG tree
    std::list<RT::AbstractLight const *>	light;		// List of light
    std::string					file;		// Master file of scene
    std::list<std::string>			dependencies;	// List of dependencies of scene
    Config					config;
  };
};

#endif