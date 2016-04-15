#include <sstream>

#include "Config.hpp"
#include "Scene.hpp"
#include "Window.hpp"

RT::Scene::Scene()
  : _camera(Math::Matrix<4, 4>::identite()), _csg(nullptr), _light(nullptr)
{
  _image.create(RT::Config::Window::Width, RT::Config::Window::Height, RT::Color(0.084f).sfml());
}

RT::Scene::~Scene()
{}

void	RT::Scene::clear()
{
  delete _csg;
  delete _light;

  *this = RT::Scene();
}
