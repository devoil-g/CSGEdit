#include <ctime>
#include <iostream>

#ifdef _WIN32
#include <stdexcept>
#include <windows.h>
#endif

#include "AdvancedRenderer.hpp"
#include "Config.hpp"

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

  // Reset true color image
  _image.clear();
  _image.resize(_scene->image().getSize().x * _scene->image().getSize().y, RT::Color(0.f));
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
	if (sample == 0)
	{
	  RT::Ray	r;

	  r.d().x() = _scene->image().getSize().x;
	  r.d().y() = _scene->image().getSize().x / 2.f - (x + a) + 0.5f;
	  r.d().z() = _scene->image().getSize().y / 2.f - (y + b) + 0.5f;

	  _image[(y + b) * _scene->image().getSize().x + x + a] = renderVirtualReality(r);
	  _scene->image().setPixel(x + a, y + b, _image[(y + b) * _scene->image().getSize().x + x + a].sfml());

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

	  _image[(y + b) * _scene->image().getSize().x + x + a] = (_image[(y + b) * _scene->image().getSize().x + x + a] + clr / std::pow(2, sample - 1)) / 2.f;
	  _scene->image().setPixel(x + a, y + b, _image[(y + b) * _scene->image().getSize().x + x + a].sfml());

	  ray += (unsigned int)std::pow(2, sample - 1);
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

  // Validate changes only if not interrupted
  if (active())
  {
    _grid[zone] = sample + 1;
    _ray += ray;
  }
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
    return renderCamera(ray);

  // Generate random position on aperture
  double		angle = Math::Random::rand(2.f * Math::Pi);
  double		distance = std::sqrt(Math::Random::rand(1.f)) * _scene->dof().aperture / 2.f;
  RT::Ray		deph;

  deph.p().x() = ray.p().x();
  deph.p().y() = std::cos(angle) * distance + ray.p().y();
  deph.p().z() = std::sin(angle) * distance + ray.p().z();
  
  deph.d() = ray.p() + ray.d() * _scene->dof().focal - deph.p();

  return renderCamera(deph);
}

RT::Color		RT::AdvancedRenderer::renderCamera(RT::Ray const & ray) const
{
  return renderRay((_scene->camera() * ray).normalize());
}

RT::Color		RT::AdvancedRenderer::renderRay(RT::Ray const & ray, RT::Color mask, unsigned int bounces) const
{
  // Stop if maximum number of bounces reached
  if (bounces > RT::Config::Renderer::Advanced::MaxBounces)
    return RT::Color(0.f);

  // Stop if below minimum mask value
  if (mask.r < RT::Config::Renderer::Advanced::MinMask.r && mask.g < RT::Config::Renderer::Advanced::MinMask.g && mask.b < RT::Config::Renderer::Advanced::MinMask.b)
    return RT::Color(0.f);

  std::list<RT::Intersection>	intersect = _scene->csg()->render(ray);

  // Drop intersection behind camera
  while (!intersect.empty() && intersect.front().distance < 0.f)
    intersect.pop_front();

  // Return if no intersection
  if (intersect.empty())
    return RT::Color(0.f);

  // Russian roulette shoot
  double		roulette = Math::Random::rand(1.f);

  // Transparency
  if (roulette < intersect.front().material.transparency.intensity)
    return renderTransparency(ray, intersect.front(), mask, bounces);

  roulette = (roulette - intersect.front().material.transparency.intensity) / (1.f - intersect.front().material.transparency.intensity);

  // Reflection
  if (roulette < intersect.front().material.reflection.intensity)
    return renderReflection(ray, intersect.front(), mask, bounces);

  roulette = (roulette - intersect.front().material.reflection.intensity) / (1.f - intersect.front().material.reflection.intensity);

  // Probability of remaining component
  double		total_dse = intersect.front().material.indirect.diffuse + intersect.front().material.indirect.specular + intersect.front().material.indirect.emission;

  if (total_dse <= 0.f)
    return RT::Color(0.f);

  // Diffusion
  if (roulette < intersect.front().material.indirect.diffuse / total_dse)
    return renderDiffuse(ray, intersect.front(), mask * total_dse, bounces);

  // Specular
  if (roulette < (intersect.front().material.indirect.diffuse + intersect.front().material.indirect.specular) / total_dse)
    return renderSpecular(ray, intersect.front(), mask * total_dse, bounces);

  // Emission
  return renderEmission(ray, intersect.front(), mask * total_dse, bounces);
}

RT::Color		RT::AdvancedRenderer::renderEmission(RT::Ray const & ray, RT::Intersection const & intersection, RT::Color mask, unsigned int bounces) const
{
  return mask * intersection.material.color;
}

RT::Color		RT::AdvancedRenderer::renderDiffuse(RT::Ray const & ray, RT::Intersection const & intersection, RT::Color mask, unsigned int bounces) const
{
  // Reverse normal if necessary
  RT::Ray		normal = intersection.normal;

  if (RT::Ray::cos(ray.d(), intersection.normal.d()) > 0.f)
    normal.d() *= -1.f;

  // Generate a new random ray from diffusion
  double		r1 = Math::Random::rand(2.f * Math::Pi);
  double		r2 = Math::Random::rand(1.f);
  double		r2s = std::sqrt(r2);

  Math::Vector<4>	w = normal.normalize().d();
  Math::Vector<4>	u = RT::Ray::cross((std::abs(w.x()) > 0.1f ? Math::Vector<4>(0.f, 1.f, 0.f, 0.f) : Math::Vector<4>(1.f, 0.f, 0.f, 0.f)), w);
  u /= std::sqrt(std::pow(u.x(), 2) + std::pow(u.y(), 2) + std::pow(u.z(), 2));
  Math::Vector<4>	v = RT::Ray::cross(w, u);
  Math::Vector<4>	d = u * std::cos(r1) * r2s + v * std::sin(r1) * r2s + w * std::sqrt(1.f - r2);
  
  return renderRay(RT::Ray(normal.p() + normal.d() * Math::Shift, d).normalize(), mask * intersection.material.color * RT::Ray::cos(w, d) * 2.f, bounces + 1);
}

RT::Color		RT::AdvancedRenderer::renderSpecular(RT::Ray const & ray, RT::Intersection const & intersection, RT::Color mask, unsigned int bounces) const
{
  // Reverse normal if necessary
  RT::Ray		normal = intersection.normal;

  if (RT::Ray::cos(ray.d(), intersection.normal.d()) > 0.f)
    normal.d() *= -1.f;

  RT::Ray		r = RT::Ray(normal.p() + normal.d() * Math::Shift, normal.p() - ray.p() - normal.d() * 2.f * (normal.d().x() * (normal.p().x() - ray.p().x()) + normal.d().y() * (normal.p().y() - ray.p().y()) + normal.d().z() * (normal.p().z() - ray.p().z())) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z())).normalize();

  // Generate a new random ray from specular
  double		r1 = Math::Random::rand(2.f * Math::Pi);
  double		r2 = Math::Random::rand(1.f);
  double		r2s = std::pow(r2, intersection.material.indirect.shininess / 2.f);

  Math::Vector<4>	w = r.d();
  Math::Vector<4>	u = RT::Ray::cross((std::abs(w.x()) > 0.1f ? Math::Vector<4>(0.f, 1.f, 0.f, 0.f) : Math::Vector<4>(1.f, 0.f, 0.f, 0.f)), w);
  u /= std::sqrt(std::pow(u.x(), 2) + std::pow(u.y(), 2) + std::pow(u.z(), 2));
  Math::Vector<4>	v = RT::Ray::cross(w, u);
  Math::Vector<4>	d = u * std::cos(r1) * r2s + v * std::sin(r1) * r2s + w * std::sqrt(1.f - r2);

  // Cancel if ray is under object surface
  if (RT::Ray::cos(normal.d(), d) < 0.f)
    return RT::Color(0.f);

  return renderRay(RT::Ray(r.p(), d).normalize(), mask * intersection.material.color * std::pow(RT::Ray::cos(w, d), intersection.material.indirect.shininess) * 2.f, bounces + 1);
}

RT::Color		RT::AdvancedRenderer::renderReflection(RT::Ray const & ray, RT::Intersection const & intersection, RT::Color mask, unsigned int bounces) const
{
  // Reverse normal if necessary
  RT::Ray		normal = intersection.normal;
  
  if (RT::Ray::cos(ray.d(), intersection.normal.d()) > 0.f)
    normal.d() *= -1.f;
  
  // Reflected ray
  RT::Ray		r = RT::Ray(normal.p() + normal.d() * Math::Shift, normal.p() - ray.p() - normal.d() * 2.f * (normal.d().x() * (normal.p().x() - ray.p().x()) + normal.d().y() * (normal.p().y() - ray.p().y()) + normal.d().z() * (normal.p().z() - ray.p().z())) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z())).normalize();
  
  // Apply glossiness
  if (intersection.material.reflection.glossiness != 0.f)
  {
    // Calculate rotation angles of reflected ray
    double		ry = -std::asin(r.d().z());
    double		rz = 0;

    if (r.d().x() != 0 || r.d().y() != 0)
      rz = r.d().y() > 0 ?
      +std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y())) :
      -std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y()));

    // Rotation matrix to get ray to point of view
    Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz));

    double		distance = Math::Random::rand(1.f);
    double		angle = Math::Random::rand(2.f * Math::Pi);

    // Calculate ray according to point on the hemisphere
    r.d().x() = 1.f / std::tan(Math::Utils::DegToRad(intersection.material.reflection.glossiness));
    r.d().y() = distance * std::cos(angle);
    r.d().z() = distance * std::sin(angle);
    r.d() = matrix * r.d();

    // Reflect ray if inside intersection
    if (RT::Ray::cos(r.d(), normal.d()) < 0.f)
      r.d() += normal.d() * -2.f * (r.d().x() * normal.d().x() + r.d().y() * normal.d().y() + r.d().z() * normal.d().z()) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z());
  }

  return renderRay(r, mask * intersection.material.color, bounces + 1);
}

RT::Color		RT::AdvancedRenderer::renderTransparency(RT::Ray const & ray, RT::Intersection const & intersection, RT::Color mask, unsigned int bounces) const
{
  // Fresnel
  double		cosi = RT::Ray::scalar(ray.d(), intersection.normal.d());
  double		refraction = intersection.material.transparency.refraction;

  if (cosi > 0.f)
    refraction = 1.f / refraction;
  else
    cosi *= -1.f;

  double		reflection_coef;
  double		sint = std::sqrt((std::max)((double)0.f, 1.f - cosi * cosi)) / refraction;

  // Total internal reflection
  if (sint >= 1.f - Math::Shift)
    reflection_coef = 1.f;
  else
  {
    double		cost = std::sqrt((std::max)((double)0.f, 1.f - sint * sint));
    reflection_coef = (std::pow((refraction * cosi - cost) / (refraction * cosi + cost), 2) + std::pow((cosi - refraction * cost) / (cosi + refraction * cost), 2)) / 2.f;
  }

  // Russian roulette on fresnel reflection
  if (Math::Random::rand(1.f) < reflection_coef)
  {
    RT::Intersection	new_intersection = intersection;

    // Copy transparency properties to reflection
    new_intersection.material.reflection.glossiness = intersection.material.transparency.glossiness;
    
    return renderReflection(ray, new_intersection, mask, bounces);
  }

  // Inverse normal if necessary
  RT::Ray		normal = intersection.normal;
  
  if (RT::Ray::cos(ray.d(), intersection.normal.d()) > 0.f)
    normal.d() *= -1.f;
  
  // Using sphere technique to calculate refracted ray
  std::vector<double> result = Math::solve(
    normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z(),
    2.f * (ray.d().x() * normal.d().x() + ray.d().y() * normal.d().y() + ray.d().z() * normal.d().z()),
    (ray.d().x() * ray.d().x() + ray.d().y() * ray.d().y() + ray.d().z() * ray.d().z()) * (1.f - refraction * refraction)
  );

  // Force a value in case of error
  if (result.empty())
    result.push_back(0.f);

  RT::Ray		r = RT::Ray(normal.p() - normal.d() * Math::Shift, ray.d() + normal.d() * result.front()).normalize();

  if (intersection.material.transparency.glossiness > 0.f)
  {
    // Calculate rotation angles of refracted ray
    double		ry = -std::asin(r.d().z());
    double		rz = 0;

    if (r.d().x() != 0 || r.d().y() != 0)
      rz = r.d().y() > 0 ?
      +std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y())) :
      -std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y()));

    // Rotation matrix to get ray to point of view
    Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz));

    double		distance = Math::Random::rand(1.f);
    double		angle = Math::Random::rand(2.f * Math::Pi);

    // Calculate ray according to point on the hemisphere
    r.d().x() = 1.f / std::tan(Math::Utils::DegToRad(intersection.material.reflection.glossiness));
    r.d().y() = distance * std::cos(angle);
    r.d().z() = distance * std::sin(angle);
    r.d() = matrix * r.d();

    normal.d() *= -1.f;

    // Reflect ray if inside intersection
    if (RT::Ray::cos(r.d(), normal.d()) < 0.f)
      r.d() += normal.d() * -2.f * (r.d().x() * normal.d().x() + r.d().y() * normal.d().y() + r.d().z() * normal.d().z()) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z());
  }

  return renderRay(r, mask * intersection.material.color, bounces + 1);
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

  unsigned int		sample = RT::Config::Renderer::Advanced::MaxSample + 1;

  // Get current sample per pixel
  for (unsigned int i : _grid)
    if (i <= RT::Config::Renderer::Advanced::MaxSample + 1)
      sample = (std::min)(sample, i);

  // Display progression message
  if (sample > _sample)
  {
    _sample = sample;
    _scene->image().saveToFile(RT::Config::ExecutablePath + "screenshot_" + std::to_string(std::time(nullptr)) + ".png");
    std::cout << "[Render] " << std::pow(2, _sample - 1) << " samples per pixel rendered." << std::endl;
  }

  double		result = (double)_ray / ((double)_scene->image().getSize().x * (double)_scene->image().getSize().y * (double)std::pow(2, _sample));

  if (result == 1.f)
    return 0.999f;
  else
    return result;
}
