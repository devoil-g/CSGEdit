#include <sstream>

#include "Config.hpp"
#include "Scene.hpp"
#include "Window.hpp"

RT::Scene::Scene()
{
  _image.create(RT::Config::Window::Width, RT::Config::Window::Height);
  _camera = Math::Matrix<4, 4>::identite();
  _tree = nullptr;

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
{
  delete _tree;
  while (!_light.empty())
  {
    delete _light.back();
    _light.pop_back();
  }
}

std::string		RT::Scene::dump() const
{
  std::stringstream	stream;

  // _config
  stream << "thread(" << _config.threadNumber << ");";
  stream << "antialiasing(" << _antialiasing.live - 1 << ", " << _antialiasing.post << ");";
  stream << "deph_of_field(" << _dof.aperture << ", " << _dof.focal << ", " << _dof.quality << ");";
  stream << "anaglyph_3d(" << _anaglyph.offset << ", " << _anaglyph.focal << ", " << _anaglyph.maskLeft.dump() << ", " << _anaglyph.maskRight.dump() << ");";

  stream << "resolution(" << _image.getSize().x << ", " << _image.getSize().y << ");";
  stream << "transformation(" << _camera.dump() << ");camera();end();";
  stream << _tree->dump();
  
  for (std::list<RT::AbstractLight const *>::const_iterator it = _light.begin(); it != _light.end(); it++)
    stream << (*it)->dump();

  return stream.str();
}