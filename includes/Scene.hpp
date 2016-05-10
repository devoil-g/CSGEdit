#ifndef _SCENE_HPP_
#define _SCENE_HPP_

#include <SFML/Graphics/Image.hpp>

#include "AbstractCsgTree.hpp"
#include "AbstractLightTree.hpp"
#include "Color.hpp"
#include "Config.hpp"
#include "Matrix.hpp"

namespace RT
{
  namespace Config
  {
    namespace AntiAliasing
    {
      unsigned int const	Live(1);		// Live anti-aliasing default level
      unsigned int const	Post(2);		// Post anti-aliasing default level
    };

    namespace DephOfField
    {
      double const		Aperture(0.f);		// Deph of field intensity
      double const		Focal(0.f);		// Focal distance
      unsigned int const	Quality(3);		// Quality of deph of field
    };

    namespace VirtualReality
    {
      double const		Offset(0.f);		// Distance between the eyes
      double const		Distortion(0.f);	// Perspective distortion
      double const		Center(0.f);		// Eye centering
    };
  };

  class Scene
  {
  private:
    struct Config
    {
      unsigned int	threadNumber = RT::Config::ThreadNumber - 1;	// Number of thread assigned to rendering
    };

    struct AntiAliasing
    {
      unsigned int	live = RT::Config::AntiAliasing::Live;	// Live anti-aliasing level
      unsigned int	post = RT::Config::AntiAliasing::Post;	// Post anti-aliasing level
    };

    struct DephOfField
    {
      double		aperture = RT::Config::DephOfField::Aperture;	// Deph of field aperture radius
      double		focal = RT::Config::DephOfField::Focal;		// Deph of field focal distance
      unsigned int	quality = RT::Config::DephOfField::Quality;	// Deph of field quality
    };

    struct VirtualReality
    {
      double		offset = RT::Config::VirtualReality::Offset;		// Distance between eyes
      double		distortion = RT::Config::VirtualReality::Distortion;	// Perspective distortion
      double		center = RT::Config::VirtualReality::Center;		// Eye centering
    };

    sf::Image					_image;		// Rendered image of scene
    sf::Image					_hud;		// Information layer for user
    Math::Matrix<4, 4>				_camera;	// Matrix camera
    RT::AbstractCsgTree *			_csg;		// CSG tree
    RT::AbstractLightTree *			_light;		// Light tree
    RT::Scene::Config				_config;	// System configuration
    RT::Scene::AntiAliasing			_antialiasing;	// Anti-aliasing parameters
    RT::Scene::DephOfField			_dof;		// Deph of field parameters
    RT::Scene::VirtualReality			_vr;		// Anaglyph3D parameters

  public:
    Scene();
    ~Scene();

    void	clear();	// Clear scene

    // Getter and setter for all parameters
    sf::Image &					image() { return _image; };
    sf::Image &					hud() { return _hud; };
    Math::Matrix<4, 4> &			camera() { return _camera; };
    RT::AbstractCsgTree * &			csg() { return _csg; };
    RT::AbstractLightTree * &			light() { return _light; };
    RT::Scene::Config &				config() { return _config; };
    RT::Scene::AntiAliasing &			antialiasing() { return _antialiasing; };
    RT::Scene::DephOfField &			dof() { return _dof; };
    RT::Scene::VirtualReality &			vr() { return _vr; };

    // Const getter for all parameters
    sf::Image const &				image() const { return _image; };
    sf::Image const &				hud() const { return _hud; };
    Math::Matrix<4, 4> const &			camera() const { return _camera; };
    RT::AbstractCsgTree const * const &		csg() const { return _csg; };
    RT::AbstractLightTree const * const &	light() const { return _light; };
    RT::Scene::Config const &			config() const { return _config; };
    RT::Scene::AntiAliasing const &		antialiasing() const { return _antialiasing; };
    RT::Scene::DephOfField const &		dof() const { return _dof; };
    RT::Scene::VirtualReality const &		vr() const { return _vr; };
  };
};

#endif
