#include <sstream>

#include "Config.hpp"
#include "Scene.hpp"
#include "Window.hpp"

RT::Scene::Scene()
{
  _image.create(RT::Config::Window::Width, RT::Config::Window::Height, RT::Color(0.084f).sfml());
  _camera = Math::Matrix<4, 4>::identite();
  _csg = nullptr;
  _light = nullptr;

  // Thread
  _config.threadNumber = RT::Config::ThreadNumber - 1;

  // Antialiasing
  _antialiasing.live = RT::Config::AntiAliasing::Live;
  _antialiasing.post = RT::Config::AntiAliasing::Post;

  // Deph of field
  _dof.aperture = RT::Config::DephOfField::Aperture;
  _dof.focal = RT::Config::DephOfField::Focal;
  _dof.quality = RT::Config::DephOfField::Quality;

  // 3D Anaglyph
  _anaglyph.offset = RT::Config::Anaglyph3D::Offset;
  _anaglyph.focal = RT::Config::Anaglyph3D::Focal;
  _anaglyph.maskLeft = RT::Config::Anaglyph3D::MaskLeft;
  _anaglyph.maskRight = RT::Config::Anaglyph3D::MaskRight;
}

RT::Scene::~Scene()
{}

void	RT::Scene::clear()
{
  delete _csg;
  delete _light;

  *this = RT::Scene();
}
