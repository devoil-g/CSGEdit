#include <iostream>

#ifdef _WIN32
#include <stdexcept>
#include <windows.h>
#endif

#include "AdvancedRenderer.hpp"

RT::AdvancedRenderer::AdvancedRenderer()
  : _grid(), _scene(nullptr), _ray(0), _sample(0)
{}

RT::AdvancedRenderer::~AdvancedRenderer()
{
  stop();
}

void	RT::AdvancedRenderer::load(RT::Scene * scene)
{
  stop();

  _scene = scene;

  // Reset number of sample rendered
  _ray = 0;
  _sample = 0;

  // Reset zone grid
  _grid.clear();
  _grid.resize((_scene->image().getSize().x / RT::Config::Renderer::BlockSize + (_scene->image().getSize().x % RT::Config::Renderer::BlockSize ? 1 : 0)) * (_scene->image().getSize().y / RT::Config::Renderer::BlockSize + (_scene->image().getSize().y % RT::Config::Renderer::BlockSize ? 1 : 0)), 0);
}

void			RT::AdvancedRenderer::begin()
{
  // If nothing to render, cancel
  if (_grid.size() == 0)
    return;

#ifdef _WIN32
  // Prevent the system to hibernate (Vista+ & XP requests)
  if (SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED) == NULL && SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED) == NULL)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
#endif

  std::list<std::thread>	threads;

  // Launch rendering threads
  for (unsigned int i = 0; i < _scene->config().threadNumber; i++)
    threads.push_back(std::thread((void(RT::AdvancedRenderer::*)())(&RT::AdvancedRenderer::render), this));

  // Wait for rendering threads to finish
  for (std::thread & it : threads)
    it.join();
}

void			RT::AdvancedRenderer::render()
{
  for (unsigned int a = 0; a <= RT::Config::Renderer::Advanced::MaxSample && active();)
  {
    unsigned int	r = (unsigned int)Math::Random::rand((double)_grid.size());
    unsigned int	z = (unsigned int)_grid.size();

    // Find a zone to render
    for (unsigned int b = 0; b < _grid.size() && z == _grid.size(); b++)
      if (_grid[(r + b) % _grid.size()] == a)
	z = (r + b) % _grid.size();

    // Increase sample per pixel if no zone to render
    if (z == _grid.size())
      a++;
    else
      render(z);
  }
}

void			RT::AdvancedRenderer::render(unsigned int zone)
{
  unsigned int		sample = _grid[zone];

  // Lock grid zone
  _grid[zone] = RT::Config::Renderer::Advanced::MaxSample + 2;

  // Calcul zone coordinates (x, y)
  unsigned int		x = zone % (_scene->image().getSize().x / RT::Config::Renderer::BlockSize + (_scene->image().getSize().x % RT::Config::Renderer::BlockSize ? 1 : 0)) * RT::Config::Renderer::BlockSize;
  unsigned int		y = zone / (_scene->image().getSize().x / RT::Config::Renderer::BlockSize + (_scene->image().getSize().x % RT::Config::Renderer::BlockSize ? 1 : 0)) * RT::Config::Renderer::BlockSize;

  // Draw zone box
  for (unsigned int a = 0; a < RT::Config::Renderer::BlockSize; a++)
  {
    if (x + a < _scene->image().getSize().x && y < _scene->image().getSize().y)
      _scene->hud().setPixel(x + a, y, RT::Color(1.f, 0.f, 0.f).sfml());
    if (x + a < _scene->image().getSize().x && y + RT::Config::Renderer::BlockSize - 1 < _scene->image().getSize().y)
      _scene->hud().setPixel(x + a, y + RT::Config::Renderer::BlockSize - 1, RT::Color(1.f, 0.f, 0.f).sfml());
    if (x < _scene->image().getSize().x && y + a < _scene->image().getSize().y)
      _scene->hud().setPixel(x, y + a, RT::Color(1.f, 0.f, 0.f).sfml());
    if (x + RT::Config::Renderer::BlockSize - 1 < _scene->image().getSize().x && y + a < _scene->image().getSize().y)
      _scene->hud().setPixel(x + RT::Config::Renderer::BlockSize - 1, y + a, RT::Color(1.f, 0.f, 0.f).sfml());
  }

  // To count the number of ray rendered
  unsigned int		ray = 0;

  // Render zone
  for (unsigned int a = 0; a < RT::Config::Renderer::BlockSize && active(); a++)
    for (unsigned int b = 0; b < RT::Config::Renderer::BlockSize && active(); b++)
      if (x + a < _scene->image().getSize().x && y + b < _scene->image().getSize().y)
      {
	if (sample == 0)
	{
	  RT::Ray	r;

	  r.d().x() = _scene->image().getSize().x;
	  r.d().y() = _scene->image().getSize().x / 2.f - (x + a) + 0.5f;
	  r.d().z() = _scene->image().getSize().y / 2.f - (y + b) + 0.5f;

	  _scene->image().setPixel(x + a, y + b, renderVirtualReality(r).sfml());

	  ray += 1;
	}
	else
	{
	  RT::Color	clr = RT::Color(0.f);
	  RT::Ray	r;

	  r.d().x() = _scene->image().getSize().x;

	  for (unsigned int c = 0; c < std::pow(2, sample - 1); c++)
	  {
	    r.d().y() = _scene->image().getSize().x / 2.f - (x + a) + Math::Random::rand(1.f);
	    r.d().z() = _scene->image().getSize().y / 2.f - (y + b) + Math::Random::rand(1.f);

	    clr += renderVirtualReality(r);
	  }

	  _scene->image().setPixel(x + a, y + b, RT::Color((RT::Color(_scene->image().getPixel(x + a, y + b)) + clr / std::pow(2, sample - 1)) / 2.f).sfml());

	  ray += (unsigned int)std::pow(2, sample - 1);
	}
      }

  // Erase zone box
  for (unsigned int a = 0; a < RT::Config::Renderer::BlockSize; a++)
  {
    if (x + a < _scene->image().getSize().x && y < _scene->image().getSize().y)
      _scene->hud().setPixel(x + a, y, RT::Color(0.f).sfml(0.f));
    if (x + a < _scene->image().getSize().x && y + RT::Config::Renderer::BlockSize - 1 < _scene->image().getSize().y)
      _scene->hud().setPixel(x + a, y + RT::Config::Renderer::BlockSize - 1, RT::Color(0.f).sfml(0.f));
    if (x < _scene->image().getSize().x && y + a < _scene->image().getSize().y)
      _scene->hud().setPixel(x, y + a, RT::Color(0.f).sfml(0.f));
    if (x + RT::Config::Renderer::BlockSize - 1 < _scene->image().getSize().x && y + a < _scene->image().getSize().y)
      _scene->hud().setPixel(x + RT::Config::Renderer::BlockSize - 1, y + a, RT::Color(0.f).sfml(0.f));
  }

  // Add rendered ray to counter
  _ray += ray;

  // Validate changes only if not interrupted
  if (active())
    _grid[zone] = sample + 1;
  else
    _grid[zone] = sample;
}

RT::Color		RT::AdvancedRenderer::renderVirtualReality(RT::Ray const & ray) const
{
  // If no offset between eyes, skip VR
  if (_scene->vr().offset == 0.f)
    return renderDephOfField(ray.normalize());

  RT::Ray		vr = ray;
  double		center;

  // Left eye
  if (ray.d().y() > 0.f)
  {
    center = +(double)_scene->image().getSize().x * _scene->vr().center / 4.f;
    vr.p().y() = +_scene->vr().offset / 2.f;
    vr.d().y() = ray.d().y() - _scene->image().getSize().x / 4.f;
  }
  // Right eye
  else
  {
    center = -(double)_scene->image().getSize().x * _scene->vr().center / 4.f;
    vr.p().y() = -_scene->vr().offset / 2.f;
    vr.d().y() = ray.d().y() + _scene->image().getSize().x / 4.f;
  }

  // Distortion
  if (_scene->vr().distortion != 0.f)
  {
    double  distortion = std::sqrt(std::pow(vr.d().y() - center, 2) + std::pow(vr.d().z(), 2)) / (std::sqrt(std::pow(_scene->image().getSize().x, 2) + std::pow(_scene->image().getSize().y, 2)) / std::abs(_scene->vr().distortion));

    if (_scene->vr().distortion > 0.f)
      distortion = distortion / std::atan(distortion);
    else
      distortion = std::atan(distortion) / distortion;

    vr.d().y() = (vr.d().y() - center) * distortion + center;
    vr.d().z() *= distortion;
  }

  return renderDephOfField(vr.normalize());
}

RT::Color		RT::AdvancedRenderer::renderDephOfField(RT::Ray const & ray) const
{
  // If no deph of field, just return rendered ray
  if (_scene->dof().aperture == 0)
    return renderRay(ray);

  // Generate random position on aperture
  double		angle = Math::Random::rand(2.f * Math::Pi);
  double		distance = std::sqrt(Math::Random::rand(1.f)) * _scene->dof().aperture / 2.f;
  RT::Ray		deph;

  deph.p().x() = ray.p().x();
  deph.p().y() = std::cos(angle) * distance + ray.p().y();
  deph.p().z() = std::sin(angle) * distance + ray.p().z();
  
  deph.d() = ray.p() + ray.d() * _scene->dof().focal - deph.p();

  return renderRay(deph.normalize());
}

RT::Color		RT::AdvancedRenderer::renderRay(RT::Ray const & ray) const
{
  RT::Color		mask = RT::Color(1.f);
  RT::Ray		r = (_scene->camera() * ray).normalize();

  for (unsigned int bounces = 0; bounces < RT::Config::Renderer::Advanced::MaxBounce && mask != RT::Color(0.f); bounces++)
  {
    // Render intersections list with CSG tree
    std::list<RT::Intersection>	intersect = _scene->csg()->render(r);

    // Drop intersection behind camera
    while (!intersect.empty() && intersect.front().distance < 0.f)
      intersect.pop_front();

    // Return if no intersection
    if (intersect.empty())
      return RT::Color(0.f);

    // Return ray color if light emitter encountered
    if (intersect.front().material.indirect.emission != RT::Color(0.f))
      return mask * intersect.front().material.indirect.emission;

    // Generate a new random ray from diffusion
    double		r1 = Math::Random::rand(2.f * Math::Pi);
    double		r2 = Math::Random::rand(1.f);
    double		r2s = std::sqrt(r2);

    Math::Vector<4>	w = intersect.front().normal.normalize().d();
    Math::Vector<4>	u = RT::Ray::cross((std::abs(w.x()) > 0.1f ? Math::Vector<4>(0.f, 1.f, 0.f, 0.f) : Math::Vector<4>(1.f, 0.f, 0.f, 0.f)), w);
    u /= std::sqrt(std::pow(u.x(), 2) + std::pow(u.y(), 2) + std::pow(u.z(), 2));
    Math::Vector<4>	v = RT::Ray::cross(w, u);
    
    Math::Vector<4>	d = u * std::cos(r1) * r2s + v * std::sin(r1) * r2s +  w * std::sqrt(1.f - r2);
    d /= std::sqrt(std::pow(d.x(), 2) + std::pow(d.y(), 2) + std::pow(d.z(), 2));

    r.p() = intersect.front().normal.p() + intersect.front().normal.d() * Math::Shift;
    r.d() = d;

    // Update mask
    mask *= intersect.front().material.color;
    mask *= RT::Ray::cos(r.d(), intersect.front().normal.d());
    mask *= 2.f;
  }

  return RT::Color(0.f);
}

double			RT::AdvancedRenderer::progress()
{
  // Check if rendering is completed
  bool			finished = true;

  for (unsigned int i : _grid)
    if (i != RT::Config::Renderer::Advanced::MaxSample + 1)
    {
      finished = false;
      break;
    }

  if (finished == true)
    return 1.f;

  unsigned int		sample = 0;

  // Get current sample per pixel
  for (unsigned int i : _grid)
    if (i <= RT::Config::Renderer::Advanced::MaxSample + 1)
      sample = (std::max)(sample, i);

  // Display progression message
  if (sample > _sample)
  {
    _sample = sample;
    if (_sample <= RT::Config::Renderer::Advanced::MaxSample && _sample > 1)
      std::cout << "[Render] " << std::pow(2, _sample - 2) << " samples per pixel rendered." << std::endl;
  }

  double		result = (double)_ray / ((double)_scene->image().getSize().x * (double)_scene->image().getSize().y * (double)std::pow(2, _sample - 1));

  if (result == 1.f)
    return 0.999f;
  else
    return result;
}
