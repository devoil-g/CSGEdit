#include "Config.hpp"
#include "Scene.hpp"

RT::Scene::Scene()
{
  image.create(RT::Config::WindowWidth, RT::Config::WindowHeight);
  camera = Math::Matrix<4, 4>::identite();
  tree = nullptr;
  file = "";

  config.liveAntiAliasing = RT::Config::LiveAntiAliasing;
  config.postAntiAliasing = RT::Config::PostAntiAliasing;
  config.lightAmbient = RT::Config::Light::Ambient;
  config.lightDiffuse = RT::Config::Light::Diffuse;
  config.lightSpecular = RT::Config::Light::Specular;
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