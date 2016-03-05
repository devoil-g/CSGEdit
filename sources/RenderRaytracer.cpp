#include <iostream>
#include <list>

#include "Config.hpp"
#include "Math.hpp"
#include "RenderRaytracer.hpp"

RT::RenderRaytracer::RenderRaytracer()
  : _grid(), _antialiasing(), _scene(nullptr), _status(First)
{}

RT::RenderRaytracer::~RenderRaytracer()
{
  stop();
}

void	RT::RenderRaytracer::load(RT::Scene * scene)
{
  stop();

  _scene = scene;

  // Set status to first pass
  _status = First;
  _progress = 0;
  
  if (_scene == nullptr || _scene->tree() == nullptr || _scene->light().empty())
  {
    _grid.resize(0);
    _antialiasing.resize(0);
  }
  else
  {
    // Reset zone grid
    _grid.resize((_scene->image().getSize().x / RT::Config::Raytracer::BlockSize + (_scene->image().getSize().x % RT::Config::Raytracer::BlockSize ? 1 : 0)) * (_scene->image().getSize().y / RT::Config::Raytracer::BlockSize + (_scene->image().getSize().y % RT::Config::Raytracer::BlockSize ? 1 : 0)));
    for (unsigned int i = 0; i < _grid.size(); i++)
      _grid[i] = RT::Config::Raytracer::BlockSize;

    // Reset antialiasing factor
    _antialiasing.resize(_scene->image().getSize().x * _scene->image().getSize().y);
    for (unsigned int i = 0; i < _antialiasing.size(); i++)
      _antialiasing[i] = _scene->antialiasing().live;
  }
}

void	RT::RenderRaytracer::begin()
{
  // If nothing to render, cancel
  if (_grid.size() == 0)
    return;

  std::list<std::thread>  threads;

  // Launch rendering threads
  for (unsigned int i = 0; i < _scene->config().threadNumber; i++)
    threads.push_back(std::thread((void(RT::RenderRaytracer::*)())(&RT::RenderRaytracer::render), this));

  // Wait for rendering threads to finish
  while (!threads.empty())
  {
    threads.front().join();
    threads.pop_front();
  }

  // If first pass done, initiate second pass
  if (_status == First)
  {
    unsigned int  total = 0;

    for (unsigned int a = 0; a < _grid.size(); a++)
      if (_grid[a] == 0)
	total++;

    if (total == _grid.size())
    {
      std::cout << "[Render] First pass completed, begining second pass." << std::endl;
      antialiasing();
      for (unsigned int i = 0; i < _grid.size(); i++)
	_grid[i] = 1;
      _status = Second;
      _progress = 0;
      begin();
    }
  }
}

void	RT::RenderRaytracer::antialiasing()
{
  std::vector<double> sobel;
  double	      sobel_min = +std::numeric_limits<double>::max();
  double	      sobel_max = -std::numeric_limits<double>::max();

  sobel.resize(_scene->image().getSize().x * _scene->image().getSize().y);
  for (unsigned int x = 1; x < _scene->image().getSize().x - 1; x++)
    for (unsigned int y = 1; y < _scene->image().getSize().y - 1; y++)
    {
      double  h_r, h_g, h_b;
      double  v_r, v_g, v_b;

      // Rendering sobel horiontally and verticaly for each color component
      h_r = RT::Color(_scene->image().getPixel(x - 1, y - 1)).r * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y - 1)).r * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y - 1)).r * 1.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 0)).r * -2.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 0)).r * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 0)).r * 2.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 1)).r * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 1)).r * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 1)).r * 1.f;

      h_g = RT::Color(_scene->image().getPixel(x - 1, y - 1)).g * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y - 1)).g * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y - 1)).g * 1.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 0)).g * -2.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 0)).g * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 0)).g * 2.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 1)).g * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 1)).g * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 1)).g * 1.f;

      h_b = RT::Color(_scene->image().getPixel(x - 1, y - 1)).b * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y - 1)).b * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y - 1)).b * 1.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 0)).b * -2.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 0)).b * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 0)).b * 2.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 1)).b * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 1)).b * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 1)).b * 1.f;

      v_r = RT::Color(_scene->image().getPixel(x - 1, y - 1)).r * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y - 1)).r * -2.f
	+ RT::Color(_scene->image().getPixel(x + 1, y - 1)).r * -1.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 0)).r * 0.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 0)).r * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 0)).r * 0.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 1)).r * 1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 1)).r * 2.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 1)).r * 1.f;

      v_g = RT::Color(_scene->image().getPixel(x - 1, y - 1)).g * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y - 1)).g * -2.f
	+ RT::Color(_scene->image().getPixel(x + 1, y - 1)).g * -1.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 0)).g * 0.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 0)).g * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 0)).g * 0.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 1)).g * 1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 1)).g * 2.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 1)).g * 1.f;

      v_b = RT::Color(_scene->image().getPixel(x - 1, y - 1)).b * -1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y - 1)).b * -2.f
	+ RT::Color(_scene->image().getPixel(x + 1, y - 1)).b * -1.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 0)).b * 0.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 0)).b * 0.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 0)).b * 0.f
	+ RT::Color(_scene->image().getPixel(x - 1, y + 1)).b * 1.f
	+ RT::Color(_scene->image().getPixel(x + 0, y + 1)).b * 2.f
	+ RT::Color(_scene->image().getPixel(x + 1, y + 1)).b * 1.f;

      // Combining squared components
      sobel[x + y * _scene->image().getSize().x] = std::sqrt(h_r * h_r + h_g * h_g + h_b * h_b + v_r * v_r + v_g * v_g + v_b * v_b);

      // Get minimum and maximum values
      sobel_min = sobel_min < sobel[x + y * _scene->image().getSize().x] ? sobel_min : sobel[x + y * _scene->image().getSize().x];
      sobel_max = sobel_max > sobel[x + y * _scene->image().getSize().x] ? sobel_max : sobel[x + y * _scene->image().getSize().x];
    }

  // If no edge detected, cancel
  if (sobel_max - sobel_min == 0)
    return;

  // Set antialiasing to maximum for borders
  for (unsigned int i = 0; i < _antialiasing.size(); i++)
    _antialiasing[i] = _scene->antialiasing().post;

  // Apply sobel to antialiasing image
  for (int a = _scene->antialiasing().post; a >= 0; a--)
    for (unsigned int x = 1; x < _scene->image().getSize().x - 1; x++)
      for (unsigned int y = 1; y < _scene->image().getSize().y - 1; y++)
	if ((sobel[x + y * _scene->image().getSize().x] - sobel_min) / (sobel_max - sobel_min) < std::pow(1.f / 6.4f, _scene->antialiasing().post - a))
	  _antialiasing[x + y * _scene->image().getSize().x] = a;
}

void	RT::RenderRaytracer::render()
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
      render(z);
  }
}

void	RT::RenderRaytracer::render(unsigned int zone)
{
  unsigned int	size = _grid[zone];
  
  // Lock grid zone
  _grid[zone] = RT::Config::Raytracer::BlockSize + 1;

  // Calcul zone coordinates (x, y)
  unsigned int	x = zone % (_scene->image().getSize().x / RT::Config::Raytracer::BlockSize + (_scene->image().getSize().x % RT::Config::Raytracer::BlockSize ? 1 : 0)) * RT::Config::Raytracer::BlockSize;
  unsigned int	y = zone / (_scene->image().getSize().x / RT::Config::Raytracer::BlockSize + (_scene->image().getSize().x % RT::Config::Raytracer::BlockSize ? 1 : 0)) * RT::Config::Raytracer::BlockSize;
  
  // To count the number of pixels rendered
  unsigned int	p = 0;

  // Render zone
  for (unsigned int a = 0; a < RT::Config::Raytracer::BlockSize && active(); a += size)
    for (unsigned int b = 0; b < RT::Config::Raytracer::BlockSize && active(); b += size)
      if (x + a < _scene->image().getSize().x && y + b < _scene->image().getSize().y)
      {
	if (_status == First)
	{
	  if ((size == RT::Config::Raytracer::BlockSize || a % (size * 2) != 0 || b % (size * 2) != 0))
	  {
	    RT::Color clr = renderAntialiasing(x + a, y + b, _scene->antialiasing().live);

	    p += _scene->antialiasing().live * _scene->antialiasing().live;
	    for (unsigned int c = 0; c < size; c++)
	      for (unsigned int d = 0; d < size; d++)
		if (x + a + c < _scene->image().getSize().x && y + b + d < _scene->image().getSize().y)
		  _scene->image().setPixel(x + a + c, y + b + d, clr.sfml());
	  }
	}
	else
	{
	  if (_antialiasing[(x + a) + (y + b) * _scene->image().getSize().x] > 0)
	  {
	    _scene->image().setPixel(x + a, y + b, renderAntialiasing(x + a, y + b, _scene->antialiasing().live + _antialiasing[(x + a) + (y + b) * _scene->image().getSize().x]).sfml());
	    p += (_scene->antialiasing().live + _antialiasing[(x + a) + (y + b) * _scene->image().getSize().x]) * (_scene->antialiasing().live + _antialiasing[(x + a) + (y + b) * _scene->image().getSize().x]);
	  }
	}
      }

  // Validate changes only if not interrupted
  if (active())
  {
    _grid[zone] = size / 2;
    _progress += p;
  }
  else
    _grid[zone] = size;
}

RT::Color RT::RenderRaytracer::renderAntialiasing(unsigned int x, unsigned int y, unsigned int antialiasing) const
{
  RT::Color	clr;
  
  // Divide a pixel to antialiasing² sub-pixels to avoid aliasing
  for (unsigned int a = 0; a < antialiasing; a++)
    for (unsigned int b = 0; b < antialiasing; b++)
    {
      RT::Ray	ray;
      
      // Calculate sub-pixel ray
      ray.d().x() = _scene->image().getSize().x;
      ray.d().y() = _scene->image().getSize().x / 2.f - x + (2.f * a + 1) / (2.f * antialiasing);
      ray.d().z() = _scene->image().getSize().y / 2.f - y + (2.f * b + 1) / (2.f * antialiasing);

      // Sum rendered color
      clr += renderAnaglyph3D(ray.normalize());
    }

  // Return average color
  return clr / (antialiasing * antialiasing);
}

RT::Color RT::RenderRaytracer::renderAnaglyph3D(RT::Ray const & ray) const
{
  // If no offset between eyes, do not render anaglyph
  if (_scene->anaglyph().offset == 0)
    return renderDephOfField((_scene->camera() * ray).normalize());

  // Calculate focus angle
  double		angle = Math::Utils::RadToDeg(Math::Pi / 2.f - std::atan(_scene->anaglyph().focal / (_scene->anaglyph().offset / 2.f)));

  // Calculate left/right eye ray accoding offset and focus angle
  Math::Matrix<4, 4>	cam_left = _scene->camera() * Math::Matrix<4, 4>::translation(0.f, +_scene->anaglyph().offset / 2.f, 0.f) * Math::Matrix<4, 4>::rotation(0, 0, -angle);
  Math::Matrix<4, 4>	cam_right = _scene->camera() * Math::Matrix<4, 4>::translation(0.f, -_scene->anaglyph().offset / 2.f, 0.f) * Math::Matrix<4, 4>::rotation(0, 0, +angle);

  // Render left/right colors
  RT::Color		clr_left = renderDephOfField((cam_left * ray).normalize());
  RT::Color		clr_right = renderDephOfField((cam_right * ray).normalize());

  // Return color using masks anaglyph MaskLeft/Right from scene configuration
  return (clr_left * _scene->anaglyph().maskLeft) + (clr_right * _scene->anaglyph().maskRight);
}

RT::Color RT::RenderRaytracer::renderDephOfField(RT::Ray const & ray) const
{
  // If no deph of field, just return rendered ray
  if (_scene->dof().quality <= 1 || _scene->dof().aperture == 0)
    return renderRay(ray);

  unsigned int	      nb_ray = 0;
  Math::Matrix<4, 4>  rot = Math::Matrix<4, 4>::rotation(Math::Random::rand(360.f), Math::Random::rand(360.f), Math::Random::rand(360.f));
  RT::Ray	      pt = Math::Matrix<4, 4>::translation(ray.d().x() * _scene->dof().focal, ray.d().y() * _scene->dof().focal, ray.d().z() * _scene->dof().focal) * ray;
  RT::Color	      clr;

  // Generate multiples ray inside aperture to simulate deph of field
  for (double a = Math::Random::rand(_scene->dof().aperture / _scene->dof().quality); a < _scene->dof().aperture; a += _scene->dof().aperture / _scene->dof().quality)
    for (double b = Math::Random::rand(Math::Pi / (_scene->dof().quality * 2)) - Math::Pi / 2.f; b < Math::Pi / 2.f; b += Math::Pi / _scene->dof().quality)
    {
      int	nb = (int)(std::cos(b) * _scene->dof().quality * 2.f * (a / _scene->dof().aperture));
      if (nb > 0)
	for (double c = Math::Random::rand(Math::Pi * 2.f / nb); c < Math::Pi * 2.f; c += Math::Pi * 2.f / nb)
	{
	  RT::Ray	deph;

	  // Calcul point position in aperture sphere
	  deph.p().x() = std::sin(b) * a;
	  deph.p().y() = std::cos(c) * std::cos(b) * a;
	  deph.p().z() = std::sin(c) * std::cos(b) * a;

	  // Randomly rotate point to smooth rendered image
	  deph = rot * deph;

	  // Calculate ray
	  deph.p() = deph.p() + ray.p();
	  deph.d() = pt.p() - deph.p();
	  
	  // Sum rendered colors
	  clr += renderRay(deph.normalize());

	  nb_ray++;
	}
    }

  // Return average color
  return clr / nb_ray;
}

RT::Color RT::RenderRaytracer::renderRay(RT::Ray const & ray, unsigned int recursivite) const
{
  if (recursivite > RT::Config::Raytracer::MaxRecursivite)
    return RT::Color(0.f);

  // Render intersections list with CSG tree
  std::list<RT::Intersection>	intersect = (_scene->tree() ? _scene->tree()->render(ray) : std::list<RT::Intersection>());
  
  // Drop intersection behind camera
  while (!intersect.empty() && intersect.front().distance < 0.f)
    intersect.pop_front();

  // Calculate transparency, reflection and light
  if (!intersect.empty())
    return renderReflection(ray, intersect.front(), recursivite)
      + renderTransparency(ray, intersect.front(), recursivite)
      + renderLight(ray, intersect.front(), recursivite);
  // If no intersection, return background color (black)
  else
    return RT::Color(0.f);
}

RT::Color RT::RenderRaytracer::renderReflection(RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
{
  if (intersection.material.reflection.intensity == 0.f || intersection.material.transparency.intensity == 1.f)
    return RT::Color(0.f);

  // Reverse normal if necessary
  RT::Ray	normal = intersection.normal;
  bool		inside = false;

  if (RT::Ray::cos(ray, intersection.normal) > 0.f)
  {
    normal.d() *= -1.f;
    inside = true;
  }

  RT::Ray	r;
  r.p() = normal.p() + normal.d() * Math::Shift;
  r.d() = normal.p() - ray.p() - normal.d() * 2.f * (normal.d().x() * (normal.p().x() - ray.p().x()) + normal.d().y() * (normal.p().y() - ray.p().y()) + normal.d().z() * (normal.p().z() - ray.p().z())) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z());
  r = r.normalize();

  std::list<RT::Ray>  rays;
  
  if (inside == true || intersection.material.reflection.glossiness == 0.f || intersection.material.reflection.quality <= 1)
    rays.push_back(r);
  else
  {
    // Calculate rotation angles of normal
    double	ry = -std::asin(r.d().z());
    double	rz = 0;

    if (r.d().x() != 0 || r.d().y() != 0)
      rz = r.d().y() > 0 ?
      +std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y())) :
      -std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y()));

    // Rotation matrix to get ray to point of view
    Math::Matrix<4, 4>  matrix = Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz));

    for (double a = Math::Random::rand(1.f / (intersection.material.reflection.quality + 1)); a < 1.f; a += 1.f / (intersection.material.reflection.quality + 1))
      for (double b = Math::Random::rand((2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f)); b < 2.f * Math::Pi; b += (2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f))
      {
	// Calculate ray according to point on the hemisphere
	r.d().x() = 1.f / std::tan(Math::Utils::DegToRad(intersection.material.reflection.glossiness));
	r.d().y() = a * std::cos(b);
	r.d().z() = a * std::sin(b);
	r.d() = matrix * r.d();

	// Reflect ray if inside intersection
	if (RT::Ray::cos(r, normal) < 0.f)
	  r.d() += normal.d() * -2.f * (r.d().x() * normal.d().x() + r.d().y() * normal.d().y() + r.d().z() * normal.d().z()) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z());

	rays.push_back(r);
      }
  }

  RT::Color	clr;
  for (std::list<RT::Ray>::const_iterator it = rays.begin(); it != rays.end(); it++)
    clr += renderRay(*it, recursivite + 1);
  
  return clr / (double)rays.size() * intersection.material.color * intersection.material.reflection.intensity * (1.f - intersection.material.transparency.intensity);
}

RT::Color RT::RenderRaytracer::renderTransparency(RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
{
  if (intersection.material.transparency.intensity == 0.f)
    return RT::Color(0.f);
  
  // Inverse normal and refraction if necessary
  RT::Ray	normal = intersection.normal;
  double	refraction = intersection.material.transparency.refraction;
  bool		inside = false;

  if (RT::Ray::cos(ray, intersection.normal) > 0.f)
  {
    normal.d() *= -1.f;
    refraction = 1.f / refraction;
    inside = true;
  }

  // Using sphere technique to calculate refracted ray
  std::vector<double> result = Math::Utils::solve(
    normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z(),
    2.f * (ray.d().x() * normal.d().x() + ray.d().y() * normal.d().y() + ray.d().z() * normal.d().z()),
    ray.d().x() * ray.d().x() + ray.d().y() * ray.d().y() + ray.d().z() * ray.d().z() - (refraction * refraction * (ray.d().x() * ray.d().x() + ray.d().y() * ray.d().y() + ray.d().z() * ray.d().z()))
    );

  // Reflection if invalid refraction
  RT::Ray r;
  if (result.empty())
  {
    r.p() = normal.p() + normal.d() * Math::Shift;
    r.d() = normal.p() - ray.p() - 2.f * normal.d() * (normal.d().x() * (normal.p().x() - ray.p().x()) + normal.d().y() * (normal.p().y() - ray.p().y()) + normal.d().z() * (normal.p().z() - ray.p().z())) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z());
    recursivite += 1;
    inside = true;
  }
  else
  {
    r.p() = normal.p() - normal.d() * Math::Shift;
    r.d() = ray.d() + normal.d() * result.front();
    normal.d() *= -1.f;
  }
  r = r.normalize();

  std::list<RT::Ray>	rays;

  if (inside == true || intersection.material.transparency.glossiness == 0.f || intersection.material.transparency.quality <= 1.f)
    rays.push_back(r);
  else
  {
    // Calculate rotation angles of normal
    double	ry = -std::asin(r.d().z());
    double	rz = 0;

    if (r.d().x() != 0 || r.d().y() != 0)
      rz = r.d().y() > 0 ?
      +std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y())) :
      -std::acos(r.d().x() / std::sqrt(r.d().x() * r.d().x() + r.d().y() * r.d().y()));

    // Rotation matrix to get ray to point of view
    Math::Matrix<4, 4>  matrix = Math::Matrix<4, 4>::rotation(0, Math::Utils::RadToDeg(ry), Math::Utils::RadToDeg(rz));
    
    for (double a = Math::Random::rand(1.f / (intersection.material.transparency.quality + 1)); a < 1.f; a += 1.f / (intersection.material.transparency.quality + 1))
      for (double b = Math::Random::rand((2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f)); b < 2.f * Math::Pi; b += (2.f * Math::Pi) / (int)(2.f * a * Math::Pi + 1.f))
      {
	// Calculate ray according to point on the hemisphere
	r.d().x() = 1.f / std::tan(Math::Utils::DegToRad(intersection.material.transparency.glossiness));
	r.d().y() = a * std::cos(b);
	r.d().z() = a * std::sin(b);
	r.d() = matrix * r.d();

	// Reflect ray if inside intersection
	if (RT::Ray::cos(r, normal) < 0.f)
	  r.d() += normal.d() * -2.f * (r.d().x() * normal.d().x() + r.d().y() * normal.d().y() + r.d().z() * normal.d().z()) / (normal.d().x() * normal.d().x() + normal.d().y() * normal.d().y() + normal.d().z() * normal.d().z());

	rays.push_back(r);
      }
  }

  RT::Color	clr;
  for (std::list<RT::Ray>::const_iterator it = rays.begin(); it != rays.end(); it++)
    clr += renderRay(*it, recursivite);
  
  return clr / (double)rays.size() * intersection.material.color * intersection.material.transparency.intensity;
}

RT::Color RT::RenderRaytracer::renderLight(RT::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
{
  RT::Color light;

  for (std::list<RT::AbstractLight const *>::const_iterator it = _scene->light().begin(); it != _scene->light().end(); it++)
    light += (*it)->render(_scene, ray, intersection);

  return light;
}

double	  RT::RenderRaytracer::progress() const
{
  if (_status == First)
  {
    unsigned int	total = 0;

    for (unsigned int a = 0; a < _grid.size(); a++)
      if (_grid[a] == 0)
	total++;

    if (total == _grid.size())
      return 0.999f;
    else
    {
      double		progress = (double)_progress / (double)(_scene->image().getSize().x * _scene->image().getSize().y * _scene->antialiasing().live * _scene->antialiasing().live);

      return progress == 1.f ? 0.999f : progress;
    }
  }
  else
  {
    unsigned int	total = 0;

    for (unsigned int a = 0; a < _grid.size(); a++)
      if (_grid[a] == 0)
	total++;

    if (total == _grid.size())
      return 1.f;

    total = 0;
    for (unsigned int x = 0; x < _scene->image().getSize().x; x++)
      for (unsigned int y = 0; y < _scene->image().getSize().y; y++)
	if (_antialiasing[x + y * _scene->image().getSize().x] > 0)
	  total += (_antialiasing[x + y * _scene->image().getSize().x] + _scene->antialiasing().live) * (_antialiasing[x + y * _scene->image().getSize().x] + _scene->antialiasing().live);

    return (double)((_scene->image().getSize().x * _scene->image().getSize().y * _scene->antialiasing().live * _scene->antialiasing().live) + _progress) / (double)((_scene->image().getSize().x * _scene->image().getSize().y * _scene->antialiasing().live * _scene->antialiasing().live) + total);
  }
}