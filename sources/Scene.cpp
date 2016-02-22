#include <sstream>

#include "Config.hpp"
#include "Scene.hpp"

RT::Scene::Scene()
{
  image.create(RT::Config::WindowWidth, RT::Config::WindowHeight);
  camera = Math::Matrix<4, 4>::identite();
  tree = nullptr;

  // Thread
  config.threadNumber = RT::Config::ThreadNumber;

  // Antialiasing
  config.liveAntiAliasing = RT::Config::LiveAntiAliasing;
  config.postAntiAliasing = RT::Config::PostAntiAliasing;

  // Global light multiplier
  config.lightAmbient = RT::Config::Light::Ambient;
  config.lightDiffuse = RT::Config::Light::Diffuse;
  config.lightSpecular = RT::Config::Light::Specular;

  // Deph of field
  config.dofAperture = RT::Config::DephOfField::Aperture;
  config.dofFocal = RT::Config::DephOfField::Focal;
  config.dofQuality = RT::Config::DephOfField::Quality;

  // 3D Anaglyph
  config.anaglyphOffset = RT::Config::Anaglyph3D::Offset;
  config.anaglyphFocal = RT::Config::Anaglyph3D::Focal;
  config.anaglyphMaskLeft = RT::Config::Anaglyph3D::MaskLeft;
  config.anaglyphMaskRight = RT::Config::Anaglyph3D::MaskRight;
}

RT::Scene::~Scene()
{
  delete tree;
  while (!light.empty())
  {
    delete light.back();
    light.pop_back();
  }
}

std::string		RT::Scene::dump() const
{
  std::stringstream	stream;

  // Config
  stream << "thread(" << config.threadNumber << ");";
  stream << "antialiasing(" << config.liveAntiAliasing << ", " << config.postAntiAliasing << ");";
  stream << "light(" << config.lightAmbient.dump() << ", " << config.lightDiffuse.dump() << ", " << config.lightSpecular.dump() << ");";
  stream << "deph_of_field(" << config.dofAperture << ", " << config.dofFocal << ", " << config.dofQuality << ");";
  stream << "anaglyph_3d(" << config.anaglyphOffset << ", " << config.anaglyphFocal << ", " << config.anaglyphMaskLeft.dump() << ", " << config.anaglyphMaskRight.dump() << ");";

  stream << "resolution(" << image.getSize().x << ", " << image.getSize().y << ");";
  stream << "transformation(" << camera.dump() << ");camera();end();";
  stream << tree->dump();
  
  for (std::list<RT::AbstractLight const *>::const_iterator it = light.begin(); it != light.end(); it++)
    stream << (*it)->dump();

  return stream.str();
}