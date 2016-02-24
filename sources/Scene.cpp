#include <sstream>

#include "Config.hpp"
#include "Scene.hpp"

RT::Scene::Scene()
{
  _image.create(RT::Config::WindowWidth, RT::Config::WindowHeight);
  _camera = Math::Matrix<4, 4>::identite();
  _tree = nullptr;

  // Thread
  _config.threadNumber = RT::Config::ThreadNumber;

  // Antialiasing
  _config.liveAntiAliasing = RT::Config::LiveAntiAliasing;
  _config.postAntiAliasing = RT::Config::PostAntiAliasing;

  // Global light multiplier
  _config.lightAmbient = RT::Config::Light::Ambient;
  _config.lightDiffuse = RT::Config::Light::Diffuse;
  _config.lightSpecular = RT::Config::Light::Specular;

  // Deph of field
  _config.dofAperture = RT::Config::DephOfField::Aperture;
  _config.dofFocal = RT::Config::DephOfField::Focal;
  _config.dofQuality = RT::Config::DephOfField::Quality;

  // 3D Anaglyph
  _config.anaglyphOffset = RT::Config::Anaglyph3D::Offset;
  _config.anaglyphFocal = RT::Config::Anaglyph3D::Focal;
  _config.anaglyphMaskLeft = RT::Config::Anaglyph3D::MaskLeft;
  _config.anaglyphMaskRight = RT::Config::Anaglyph3D::MaskRight;
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
  stream << "antialiasing(" << _config.liveAntiAliasing << ", " << _config.postAntiAliasing << ");";
  stream << "light(" << _config.lightAmbient.dump() << ", " << _config.lightDiffuse.dump() << ", " << _config.lightSpecular.dump() << ");";
  stream << "deph_of_field(" << _config.dofAperture << ", " << _config.dofFocal << ", " << _config.dofQuality << ");";
  stream << "anaglyph_3d(" << _config.anaglyphOffset << ", " << _config.anaglyphFocal << ", " << _config.anaglyphMaskLeft.dump() << ", " << _config.anaglyphMaskRight.dump() << ");";

  stream << "resolution(" << _image.getSize().x << ", " << _image.getSize().y << ");";
  stream << "transformation(" << _camera.dump() << ");camera();end();";
  stream << _tree->dump();
  
  for (std::list<RT::AbstractLight const *>::const_iterator it = _light.begin(); it != _light.end(); it++)
    stream << (*it)->dump();

  return stream.str();
}