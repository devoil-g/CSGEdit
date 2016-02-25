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
      unsigned int	threadNumber;		// Number of thread assigned to rendering
      unsigned int	liveAntiAliasing;	// Live anti-aliasing level
      unsigned int	postAntiAliasing;	// Post anti-aliasing level
      RT::Color		lightAmbient;		// Global ambient light multiplier
      RT::Color		lightDiffuse;		// Global diffuse light multiplier
      RT::Color		lightSpecular;		// Global specular light multiplier
      double		dofAperture;		// Deph of field aperture radius
      double		dofFocal;		// Deph of field focal distance
      unsigned int	dofQuality;		// Deph of field quality
      double		anaglyphOffset;		// Anaglyph distance between eyes
      double		anaglyphFocal;		// Anaglyph focal distance
      RT::Color		anaglyphMaskLeft;	// Anaglyph color mask for left eye
      RT::Color		anaglyphMaskRight;	// Anaglyph color mask for right eye
    };

    sf::Image					_image;		// Rendered image of scene
    Math::Matrix<4, 4>				_camera;	// Matrix camera
    RT::AbstractTree const *			_tree;		// CSG tree
    std::list<RT::AbstractLight const *>	_light;		// List of light
    std::list<std::string>			_dependencies;	// List of dependencies of scene
    Config					_config;

  public:
    Scene();
    ~Scene();

    // Getter and setter for all parameters
    sf::Image &						image() { return _image; };
    Math::Matrix<4, 4> &				camera() { return _camera; };
    RT::AbstractTree const * &				tree() { return _tree; };
    std::list<RT::AbstractLight const *> &		light() { return _light; };
    std::list<std::string> &				dependencies() { return _dependencies; };
    RT::Scene::Config &					config() { return _config; };

    // Const getter for all parameters
    sf::Image const &					image() const { return _image; };
    Math::Matrix<4, 4> const &				camera() const { return _camera; };
    RT::AbstractTree const * const &			tree() const { return _tree; };
    std::list<RT::AbstractLight const *> const &	light() const { return _light; };
    std::list<std::string> const &			dependencies() const { return _dependencies; };
    RT::Scene::Config const &				config() const { return _config; };

    std::string						dump() const;	// Return scene dump
  };
};

#endif