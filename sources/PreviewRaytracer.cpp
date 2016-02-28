// TODO: preview cache, move camera using mouse

#include <list>

#include "Config.hpp"
#include "Math.hpp"
#include "PreviewRaytracer.hpp"

RT::PreviewRaytracer::PreviewRaytracer()
  : _grid(), _scene(nullptr)
{}

RT::PreviewRaytracer::~PreviewRaytracer()
{
  stop();
}

void	RT::PreviewRaytracer::load(RT::Scene * scene)
{
  stop();

  _scene = scene;

  if (_scene == nullptr)
  {
    _grid.resize(0);
  }
  else
  {
    // Reset image
    for (unsigned int y = 0; y < _scene->image().getSize().y; y++)
      for (unsigned int x = 0; x < _scene->image().getSize().x; x++)
	_scene->image().setPixel(x, y, RT::Color(0.084f).sfml());

    // Reset zone grid
    _grid.resize((_scene->image().getSize().x / RT::Config::Raytracer::BlockSize + (_scene->image().getSize().x % RT::Config::Raytracer::BlockSize ? 1 : 0)) * (_scene->image().getSize().y / RT::Config::Raytracer::BlockSize + (_scene->image().getSize().y % RT::Config::Raytracer::BlockSize ? 1 : 0)));
    for (unsigned int i = 0; i < _grid.size(); i++)
      _grid[i] = RT::Config::Raytracer::BlockSize;
  }
}

void	RT::PreviewRaytracer::begin()
{
  std::list<std::thread>  threads;
  
  if (_scene == nullptr)
    return;

  // Launch rendering threads
  for (unsigned int i = 0; i < _scene->config().threadNumber; i++)
    threads.push_back(std::thread((void(RT::PreviewRaytracer::*)())(&RT::PreviewRaytracer::preview), this));
  
  // Wait for rendering threads to finish
  while (!threads.empty())
  {
    threads.front().join();
    threads.pop_front();
  }
}

void	RT::PreviewRaytracer::preview()
{
  while (active())
  {
    unsigned int  r = (unsigned int)Math::Random::rand((double)_grid.size());
    unsigned int  z = (unsigned int)_grid.size();

    // Find a zone to render
    for (unsigned int a = RT::Config::Raytracer::BlockSize; a > 0 && z == _grid.size(); a /= 2)
      for (unsigned int b = 0; b < _grid.size() && z == _grid.size(); b++)
	if (_grid[(r + b) % _grid.size()] == a)
	  z = (r + b) % _grid.size();

    // Return if no zone to render
    if (z == _grid.size())
      return;
    else
      preview(z);
  }
}

void	RT::PreviewRaytracer::preview(unsigned int zone)
{
  unsigned int	size = _grid[zone];
  
  // Lock grid zone
  _grid[zone] = RT::Config::Raytracer::BlockSize + 1;

  // Calcul zone coordinates (x, y)
  unsigned int	x = zone % (_scene->image().getSize().x / RT::Config::Raytracer::BlockSize + (_scene->image().getSize().x % RT::Config::Raytracer::BlockSize ? 1 : 0)) * RT::Config::Raytracer::BlockSize;
  unsigned int	y = zone / (_scene->image().getSize().x / RT::Config::Raytracer::BlockSize + (_scene->image().getSize().x % RT::Config::Raytracer::BlockSize ? 1 : 0)) * RT::Config::Raytracer::BlockSize;

  // Render zone
  for (unsigned int a = 0; a < RT::Config::Raytracer::BlockSize && active(); a += size)
    for (unsigned int b = 0; b < RT::Config::Raytracer::BlockSize && active(); b += size)
      if ((size == RT::Config::Raytracer::BlockSize || a % (size * 2) != 0 || b % (size * 2) != 0) && x + a < _scene->image().getSize().x && y + b < _scene->image().getSize().y)
      {
	RT::Color clr = preview(x + a, y + b);

	for (unsigned int c = 0; c < size; c++)
	  for (unsigned int d = 0; d < size; d++)
	    if (x + a + c < _scene->image().getSize().x && y + b + d < _scene->image().getSize().y)
	      _scene->image().setPixel(x + a + c, y + b + d, clr.sfml());
      }

  if (active())
    _grid[zone] = size / 2;
  else
    _grid[zone] = size;
}

RT::Color	RT::PreviewRaytracer::preview(unsigned int x, unsigned int y) const
{
  RT::Ray	ray;

  // Calcul ray according to (x, y) coordinates
  ray.d().x() = (double)_scene->image().getSize().x;
  ray.d().y() = (double)_scene->image().getSize().x / 2 - x + 0.5f;
  ray.d().z() = (double)_scene->image().getSize().y / 2 - y + 0.5f;
  ray = (_scene->camera() * ray).normalize();

  // Render intersections using ray
  std::list<RT::Intersection>	intersect = (_scene->tree() ? _scene->tree()->render(ray) : std::list<RT::Intersection>());
  
  // Delete back intersections
  while (!intersect.empty() && intersect.front().distance < 0)
    intersect.pop_front();

  // Calcul intersection color if it exist, otherwise return black
  if (!intersect.empty())
  {
    RT::Color light;

    for (std::list<RT::AbstractLight const *>::const_iterator it = _scene->light().begin(); it != _scene->light().end(); it++)
      light += (*it)->preview(_scene, ray, intersect.front());

    return light;
  }
  else
    return RT::Color(0.f);
}

double	  RT::PreviewRaytracer::progress() const
{
  unsigned int  r = 0;
  unsigned int  n = 1;
  
  for (unsigned int a = RT::Config::Raytracer::BlockSize; a > 0; a /= 2)
  {
    for (unsigned int b = 0; b < _grid.size(); b++)
      if (_grid[b] == a / 2)
	r += n * n;

    n *= 2;
  }

  return (double)r / (double)(_scene->image().getSize().x * _scene->image().getSize().y);
}