#include "Config.hpp"
#include "Scene.hpp"

RT::Scene::Scene()
{}

RT::Scene::~Scene()
{
  delete tree;
  while (!light.empty())
  {
    delete light.back();
    light.pop_back();
  }
}