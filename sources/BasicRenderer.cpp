#include <iostream>

#ifdef _WIN32
#include <stdexcept>
#include <windows.h>
#endif

#include "BasicRenderer.hpp"

RT::BasicRenderer::BasicRenderer()
  : _grid(), _antialiasing(), _scene(nullptr), _rayTotal(0), _rayRendered(0), _status(First)
{}

RT::BasicRenderer::~BasicRenderer()
{
  stop();
}

void	RT::BasicRenderer::load(RT::Scene * scene)
{
  stop();

  _scene = scene;

  // Set status to first pass
  _status = First;
  _rayTotal = _scene->image().getSize().x * _scene->image().getSize().y * (unsigned int)std::pow(_scene->antialiasing().live + 1, 2);
  _rayRendered = 0;

  // Reset zone grid
  _grid.clear();
  _grid.resize((_scene->image().getSize().x / RT::Config::Renderer::BlockSize + (_scene->image().getSize().x % RT::Config::Renderer::BlockSize ? 1 : 0)) * (_scene->image().getSize().y / RT::Config::Renderer::BlockSize + (_scene->image().getSize().y % RT::Config::Renderer::BlockSize ? 1 : 0)), RT::Config::Renderer::BlockSize);
  
  // Reset antialiasing factor
  _antialiasing.clear();
  _antialiasing.resize(_scene->image().getSize().x * _scene->image().getSize().y, 1);
}

void			RT::BasicRenderer::begin()
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
    threads.push_back(std::thread((void(RT::BasicRenderer::*)())(&RT::BasicRenderer::render), this));

  // Wait for rendering threads to finish
  for (std::thread & it : threads)
    it.join();

  // If first pass done, initiate second pass
  if (_status == First && _grid.size() != 0)
  {
    // Check if the first pass has been completed
    for (unsigned int i : _grid)
      if (i != 0)
	return;

    // Begin second pass
    std::cout << "[Render] First pass completed, begining second pass." << std::endl;
    antialiasing();

    // Re-lauch rendering
    begin();
  }
}

void			RT::BasicRenderer::antialiasing()
{
  std::vector<double>	sobel;
  double		sobel_min = +(std::numeric_limits<double>::max)();
  double		sobel_max = -(std::numeric_limits<double>::max)();

  sobel.resize(_scene->image().getSize().x * _scene->image().getSize().y);
  for (unsigned int x = 1; x < _scene->image().getSize().x - 1; x++)
    for (unsigned int y = 1; y < _scene->image().getSize().y - 1; y++)
    {
      // Rendering sobel horiontally and verticaly for each color component
      RT::Color	h = RT::Color(_scene->image().getPixel(x - 1, y - 1)) * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y - 1)) * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y - 1)) * 1.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 0)) * -2.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 0)) * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 0)) * 2.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 1)) * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 1)) * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 1)) * 1.f;
      RT::Color	v = RT::Color(_scene->image().getPixel(x - 1, y - 1)) * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y - 1)) * -2.f
	+ RT::Color(_scene->image().getPixel(x + 1, y - 1)) * -1.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 0)) * 0.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 0)) * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 0)) * 0.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 1)) * 1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 1)) * 2.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 1)) * 1.f;

      // Summing squared sobel
      RT::Color	clr = h * h + v * v;

      // Combining components
      sobel[x + y * _scene->image().getSize().x] = std::sqrt(clr.r + clr.g + clr.b);

      // Get minimum and maximum values
      sobel_min = (std::min)(sobel_min, sobel[x + y * _scene->image().getSize().x]);
      sobel_max = (std::max)(sobel_max, sobel[x + y * _scene->image().getSize().x]);
    }

  // If no edge detected, cancel
  if (sobel_max - sobel_min == 0)
    return;

  // Set antialiasing to maximum for borders
  std::fill(_antialiasing.begin(), _antialiasing.end(), _scene->antialiasing().post);
  
  // Apply sobel to antialiasing image
  for (int a = _scene->antialiasing().post; a >= 0; a--)
    for (unsigned int x = 1; x < _scene->image().getSize().x - 1; x++)
      for (unsigned int y = 1; y < _scene->image().getSize().y - 1; y++)
	if ((sobel[x + y * _scene->image().getSize().x] - sobel_min) / (sobel_max - sobel_min) < std::pow(1.f / 6.4f, _scene->antialiasing().post - a))
	{
	  _antialiasing[x + y * _scene->image().getSize().x] = a;
	  _scene->hud().setPixel(x, y, RT::Color(0.f, 0.f, 1.f).sfml(1.f / (_scene->antialiasing().post + 1) * a));
	}

  // Setup configuration for second pass
  std::fill(_grid.begin(), _grid.end(), 1);
  _status = Second;

  // Add post anti-aliasing ray to total
  for (unsigned int aa : _antialiasing)
    if (aa != 0)
      _rayTotal += (unsigned int)std::pow(_scene->antialiasing().live + 1 + aa, 2);
}

void			RT::BasicRenderer::render()
{
  while (active())
  {
    unsigned int	r = (unsigned int)Math::Random::rand((double)_grid.size());
    unsigned int	z = (unsigned int)_grid.size();

    // Find a zone to render
    for (unsigned int a = RT::Config::Renderer::BlockSize; a > 0 && z == _grid.size(); a /= 2)
      for (unsigned int b = 0; b < _grid.size() && z == _grid.size(); b++)
	if (_grid[(r + b) % _grid.size()] == a)
	  z = (r + b) % _grid.size();

    // Return if no zone to render
    if (z == _grid.size())
      return;
    else
      render(z);
  }
}

void			RT::BasicRenderer::render(unsigned int zone)
{
  unsigned int		size = _grid[zone];

  // Lock grid zone
  _grid[zone] = RT::Config::Renderer::BlockSize + 1;

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
  for (unsigned int a = 0; a < RT::Config::Renderer::BlockSize && active(); a += size)
    for (unsigned int b = 0; b < RT::Config::Renderer::BlockSize && active(); b += size)
      if (x + a < _scene->image().getSize().x && y + b < _scene->image().getSize().y && _antialiasing[(x + a) + (y + b) * _scene->image().getSize().x] > 0)
	if (_status == First)
	{
	  if ((size == RT::Config::Renderer::BlockSize || a % (size * 2) != 0 || b % (size * 2) != 0))
	  {
	    _scene->image().setPixel(x + a, y + b, renderAntialiasing(x + a, y + b, _scene->antialiasing().live).sfml());
	    ray += (unsigned int)std::pow(_scene->antialiasing().live + 1, 2);
	    _antialiasing[(x + a) + (y + b) * _scene->image().getSize().x] = 0;

	    for (unsigned int c = 0; c < size; c++)
	      for (unsigned int d = 0; d < size; d++)
		if (x + a + c < _scene->image().getSize().x && y + b + d < _scene->image().getSize().y)
		  _scene->image().setPixel(x + a + c, y + b + d, _scene->image().getPixel(x + a, y + b));
	  }
	}
	else
	{
	  _scene->image().setPixel(x + a, y + b, renderAntialiasing(x + a, y + b, _scene->antialiasing().live + _antialiasing[(x + a) + (y + b) * _scene->image().getSize().x]).sfml());
	  ray += (unsigned int)std::pow(_scene->antialiasing().live + 1 + _antialiasing[(x + a) + (y + b) * _scene->image().getSize().x], 2);
	  _antialiasing[(x + a) + (y + b) * _scene->image().getSize().x] = 0;

	  if (a != 0 && a != RT::Config::Renderer::BlockSize - 1 && b != 0 && b != RT::Config::Renderer::BlockSize - 1)
	    _scene->hud().setPixel(x + a, y + b, RT::Color(0.f).sfml(0.f));
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
  _rayRendered += ray;

  // Validate changes only if not interrupted
  if (active())
    _grid[zone] = size / 2;
  else
    _grid[zone] = size;
}

RT::Color		RT::BasicRenderer::renderAntialiasing(unsigned int x, unsigned int y, unsigned int antialiasing) const
{
  RT::Color		clr;

  // Divide a pixel to antialiasing² sub-pixels to avoid aliasing
  for (unsigned int a = 0; a < antialiasing + 1; a++)
    for (unsigned int b = 0; b < antialiasing + 1; b++)
    {
      RT::Ray		ray;

      // Calculate sub-pixel ray
      ray.d().x() = _scene->image().getSize().x;
      ray.d().y() = _scene->image().getSize().x / 2.f - x + (2.f * a + 1) / (2.f * (antialiasing + 1));
      ray.d().z() = _scene->image().getSize().y / 2.f - y + (2.f * b + 1) / (2.f * (antialiasing + 1));

      // Sum rendered color
      clr += renderVirtualReality(ray);
    }

  // Return average color
  return clr / std::pow(antialiasing + 1, 2);
}

RT::Color		RT::BasicRenderer::renderVirtualReality(RT::Ray const & ray) const
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

RT::Color		RT::BasicRenderer::renderDephOfField(RT::Ray const & ray) const
{
  // If no deph of field, just return rendered ray
  if (_scene->dof().quality <= 1 || _scene->dof().aperture == 0)
    return renderRay(ray);

  unsigned int		nb_ray = 0;
  RT::Color		clr;

  // Generate multiples ray inside aperture to simulate deph of field
  for (double distance = Math::Random::rand(1.f / _scene->dof().quality); distance < 1.f; distance += 1.f / _scene->dof().quality)
    for (double angle = Math::Random::rand(2.f * Math::Pi / ((int)(_scene->dof().quality * 2.f * distance) + 1.f)); angle < 2.f * Math::Pi; angle += 2.f * Math::Pi / ((int)(_scene->dof().quality * 2.f * distance) + 1.f))
    {
      RT::Ray	deph;

      deph.p().x() = ray.p().x();
      deph.p().y() = std::cos(angle) * distance * (_scene->dof().aperture / 2.f) + ray.p().y();
      deph.p().z() = std::sin(angle) * distance * (_scene->dof().aperture / 2.f) + ray.p().z();

      deph.d() = ray.p() + ray.d() * _scene->dof().focal - deph.p();

      // Sum rendered colors
      clr += renderRay(deph.normalize());

      nb_ray++;
    }

  // Return average color
  return clr / nb_ray;
}

RT::Color		RT::BasicRenderer::renderRay(RT::Ray const & ray, unsigned int recursivite) const
{
  if (recursivite > RT::Config::Renderer::Basic::MaxRecursivite)
    return RT::Color(0.f);

  RT::Ray		r = (_scene->camera() * ray).normalize();

  // Render intersections list with CSG tree
  std::list<RT::Intersection>	intersect = _scene->csg()->render(r);

  // Drop intersection behind camera
  while (!intersect.empty() && intersect.front().distance < 0.f)
    intersect.pop_front();

  // Calculate transparency, reflection and light
  if (!intersect.empty())
    return renderReflection(r, intersect.front(), recursivite)
    + renderTransparency(r, intersect.front(), recursivite)
    + renderLight(r, intersect.front(), recursivite);
  // If no intersection, return background color (black)
  else
    return RT::Color(0.f);
}

RT::Color		RT::BasicRenderer::renderReflection(RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
{
  if (intersection.material.reflection.intensity == 0.f || intersection.material.transparency.intensity == 1.f)
    return RT::Color(0.f);

  // Reverse normal if necessary
  RT::Ray		normal = intersection.normal;
  bool			inside = false;

  if (RT::Ray::cos(ray.d(), intersection.normal.d()) > 0.f)
  {
    normal.d() *= -1.f;
    inside = true;
  }

  RT::Ray		r = RT::Ray(normal.p() + normal.d() * Math::Shift, normal.p() - ray.p() - normal.d() * 2.f * (normal.d().x() * (normal.p().x() - ray.p().x()) + normal.d().y() * (normal.p().y() - ray.p().y()) + normal.d().z() * (normal.p().z() - ray.p().z())) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z())).normalize();
  unsigned int		quality = intersection.material.reflection.quality > recursivite ? intersection.material.reflection.quality - recursivite : 0;

  // If basic reflection, return basic rendering
  if (inside == true || intersection.material.reflection.glossiness == 0.f || quality <= 1)
    return renderRay(r, recursivite + 1) * intersection.material.color * intersection.material.reflection.intensity * (1.f - intersection.material.transparency.intensity);

  // Calculate rotation angles of normal
  double		ry = -std::asin(r.d().z());
  double		rz = 0;

  if (r.d().x() != 0 || r.d().y() != 0)
    rz = r.d().y() > 0 ?
    +std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y())) :
    -std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y()));

  // Rotation matrix to get ray to point of view
  Math::Matrix<4, 4>  matrix = Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz));

  std::list<RT::Ray>	rays;
  for (double a = Math::Random::rand(1.f / (quality + 1)); a < 1.f; a += 1.f / (quality + 1))
    for (double b = Math::Random::rand((2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f)); b < 2.f * Math::Pi; b += (2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f))
    {
      // Calculate ray according to point on the hemisphere
      r.d().x() = 1.f / std::tan(Math::Utils::DegToRad(intersection.material.reflection.glossiness));
      r.d().y() = a * std::cos(b);
      r.d().z() = a * std::sin(b);
      r.d() = matrix * r.d();

      // Reflect ray if inside intersection
      if (RT::Ray::cos(r.d(), normal.d()) < 0.f)
	r.d() += normal.d() * -2.f * (r.d().x() * normal.d().x() + r.d().y() * normal.d().y() + r.d().z() * normal.d().z()) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z());

      rays.push_back(r.normalize());
    }

  // Render all sub-rays
  RT::Color		clr;
  for (std::list<RT::Ray>::const_iterator it = rays.begin(); it != rays.end(); it++)
    clr += renderRay(*it, recursivite + 1);

  return clr / (double)rays.size() * intersection.material.color * intersection.material.reflection.intensity * (1.f - intersection.material.transparency.intensity);
}

RT::Color		RT::BasicRenderer::renderTransparency(RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
{
  if (intersection.material.transparency.intensity == 0.f)
    return RT::Color(0.f);

  // Fresnel
  double		cosi = RT::Ray::scalar(ray.d(), intersection.normal.d());
  double		refraction = intersection.material.transparency.refraction;

  if (cosi > 0.f)
    refraction = 1.f / refraction;
  else
    cosi *= -1.f;

  RT::Color		reflection_clr;
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

  // Render reflection if significant amount
  if (reflection_coef > Math::Shift)
  {
    RT::Intersection	new_intersection = intersection;

    // Copy transparency properties to reflection
    new_intersection.material.reflection.intensity = intersection.material.transparency.intensity * reflection_coef;
    new_intersection.material.reflection.glossiness = intersection.material.transparency.glossiness;
    new_intersection.material.reflection.quality = intersection.material.transparency.quality;
    // Ignore transparency
    new_intersection.material.transparency.intensity = 0.f;

    reflection_clr = renderReflection(ray, new_intersection, recursivite + 1);
  }

  // Stop immediatly if total reflection
  if (reflection_coef > 1.f - Math::Shift)
    return reflection_clr;

  // Inverse normal if necessary
  RT::Ray		normal = intersection.normal;
  bool			inside = false;

  if (RT::Ray::cos(ray.d(), intersection.normal.d()) > 0.f)
  {
    normal.d() *= -1.f;
    inside = true;
  }

  // Using sphere technique to calculate refracted ray
  std::vector<double> result = Math::solve(
    normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z(),
    2.f * (ray.d().x() * normal.d().x() + ray.d().y() * normal.d().y() + ray.d().z() * normal.d().z()),
    (ray.d().x() * ray.d().x() + ray.d().y() * ray.d().y() + ray.d().z() * ray.d().z()) * (1.f - refraction * refraction)
  );

  // Force a value in case of error
  if (result.empty())
    result.push_back(0.f);

  RT::Ray	r = RT::Ray(normal.p() - normal.d() * Math::Shift, ray.d() + normal.d() * result.front()).normalize();
  normal.d() *= -1.f;

  unsigned int		quality = intersection.material.transparency.quality > recursivite ? intersection.material.transparency.quality - recursivite : 0;

  // If basic transparency, return basic rendering
  if (inside == true || intersection.material.transparency.glossiness == 0.f || quality <= 1)
    return renderRay(r, recursivite) * intersection.material.color * (intersection.material.transparency.intensity * (1.f - reflection_coef)) + reflection_clr;

  // Calculate rotation angles of normal
  double		ry = -std::asin(r.d().z());
  double		rz = 0;

  if (r.d().x() != 0 || r.d().y() != 0)
    rz = r.d().y() > 0 ?
    +std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y())) :
    -std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y()));

  // Rotation matrix to get ray to point of view
  Math::Matrix<4, 4>	matrix = Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz));

  std::list<RT::Ray>	rays;
  for (double a = Math::Random::rand(1.f / (quality + 1)); a < 1.f; a += 1.f / (quality + 1))
    for (double b = Math::Random::rand((2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f)); b < 2.f * Math::Pi; b += (2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f))
    {
      // Calculate ray according to point on the hemisphere
      r.d().x() = 1.f / std::tan(Math::Utils::DegToRad(intersection.material.transparency.glossiness));
      r.d().y() = a * std::cos(b);
      r.d().z() = a * std::sin(b);
      r.d() = matrix * r.d();

      // Reflect ray if inside intersection
      if (RT::Ray::cos(r.d(), normal.d()) < 0.f)
	r.d() += normal.d() * -2.f * (r.d().x() * normal.d().x() + r.d().y() * normal.d().y() + r.d().z() * normal.d().z()) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z());

      rays.push_back(r.normalize());
    }

  // Render all sub-rays
  RT::Color		clr;
  for (std::list<RT::Ray>::const_iterator it = rays.begin(); it != rays.end(); it++)
    clr += renderRay(*it, recursivite + 1);

  return clr / (double)rays.size() * intersection.material.color * (intersection.material.transparency.intensity * (1.f - reflection_coef)) + reflection_clr;
}

RT::Color		RT::BasicRenderer::renderLight(RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
{
  // TODO: pass recursivite to lights to decrease quality with deph
  return _scene->light()->render(Math::Matrix<4, 4>::identite(), _scene, ray, intersection, recursivite);
}

double			RT::BasicRenderer::progress()
{
  if (_status == First)
    return ((double)_rayRendered / (double)_rayTotal) >= 1.f ? 0.999f : ((double)_rayRendered / (double)_rayTotal);
  else
  {
    for (unsigned int i : _grid)
      if (i != 0)
	return (double)_rayRendered / (double)_rayTotal;
    return 1.f;
  }
}
