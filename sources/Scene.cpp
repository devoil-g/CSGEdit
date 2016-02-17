#include "Config.hpp"
#include "Scene.hpp"

RT::Scene::Scene()
{
  image.create(RT::Config::WindowWidth, RT::Config::WindowHeight);
  camera = Math::Matrix<4, 4>::identite();
  tree = nullptr;
  file = "";

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