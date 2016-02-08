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

  if (_scene == nullptr)
  {
    _grid.resize(0);
    _antialiasing.resize(0);
  }
  else
  {
    // Reset image
    for (unsigned int y = 0; y < _scene->image.getSize().y; y++)
      for (unsigned int x = 0; x < _scene->image.getSize().x; x++)
	_scene->image.setPixel(x, y, RT::Color(0.084f).sfml());

    // Reset zone grid
    _grid.resize((_scene->image.getSize().x / RT::Config::BlockSize + (_scene->image.getSize().x % RT::Config::BlockSize ? 1 : 0)) * (_scene->image.getSize().y / RT::Config::BlockSize + (_scene->image.getSize().y % RT::Config::BlockSize ? 1 : 0)));
    for (unsigned int i = 0; i < _grid.size(); i++)
      _grid[i] = RT::Config::BlockSize;

    // Reset antialiasing factor
    _antialiasing.resize(_scene->image.getSize().x * _scene->image.getSize().y);
    for (unsigned int i = 0; i < _antialiasing.size(); i++)
      _antialiasing[i] = RT::Config::AntiAliasing;

    // Set status to first pass
    _status = First;
    _progress = 0;
  }
}

void	RT::RenderRaytracer::begin()
{
  std::list<std::thread>  threads;

  // Launch rendering threads
  for (unsigned int i = 0; i < RT::Config::ThreadNumber; i++)
    threads.push_back(std::thread((void(RT::RenderRaytracer::*)())(&RT::RenderRaytracer::render), this));

  // Wait for rendering threads to finish
  while (!threads.empty())
  {
    threads.front().join();
    threads.pop_front();
  }

  // If first pass done, initiate second pass
  if (_status == First && progress() == 0.5f)
  {
    antialiasing();
    for (unsigned int i = 0; i < _grid.size(); i++)
      _grid[i] = 1;
    _status = Second;
    _progress = 0;
    begin();
  }
}

void	RT::RenderRaytracer::antialiasing()
{
  std::vector<double> sobel;
  double	      sobel_min = +std::numeric_limits<double>::max();
  double	      sobel_max = -std::numeric_limits<double>::max();

  sobel.resize(_scene->image.getSize().x * _scene->image.getSize().y);
  for (unsigned int x = 1; x < _scene->image.getSize().x - 1; x++)
    for (unsigned int y = 1; y < _scene->image.getSize().y - 1; y++)
    {
      double  h_r, h_g, h_b;
      double  v_r, v_g, v_b;

      h_r = RT::Color(_scene->image.getPixel(x - 1, y - 1)).r * -1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y - 1)).r * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y - 1)).r * 1.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 0)).r * -2.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 0)).r * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 0)).r * 2.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 1)).r * -1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 1)).r * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 1)).r * 1.f;

      h_g = RT::Color(_scene->image.getPixel(x - 1, y - 1)).g * -1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y - 1)).g * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y - 1)).g * 1.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 0)).g * -2.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 0)).g * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 0)).g * 2.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 1)).g * -1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 1)).g * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 1)).g * 1.f;

      h_b = RT::Color(_scene->image.getPixel(x - 1, y - 1)).b * -1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y - 1)).b * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y - 1)).b * 1.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 0)).b * -2.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 0)).b * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 0)).b * 2.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 1)).b * -1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 1)).b * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 1)).b * 1.f;

      v_r = RT::Color(_scene->image.getPixel(x - 1, y - 1)).r * -1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y - 1)).r * -2.f
	+ RT::Color(_scene->image.getPixel(x + 1, y - 1)).r * -1.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 0)).r * 0.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 0)).r * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 0)).r * 0.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 1)).r * 1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 1)).r * 2.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 1)).r * 1.f;

      v_g = RT::Color(_scene->image.getPixel(x - 1, y - 1)).g * -1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y - 1)).g * -2.f
	+ RT::Color(_scene->image.getPixel(x + 1, y - 1)).g * -1.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 0)).g * 0.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 0)).g * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 0)).g * 0.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 1)).g * 1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 1)).g * 2.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 1)).g * 1.f;

      v_b = RT::Color(_scene->image.getPixel(x - 1, y - 1)).b * -1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y - 1)).b * -2.f
	+ RT::Color(_scene->image.getPixel(x + 1, y - 1)).b * -1.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 0)).b * 0.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 0)).b * 0.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 0)).b * 0.f
	+ RT::Color(_scene->image.getPixel(x - 1, y + 1)).b * 1.f
	+ RT::Color(_scene->image.getPixel(x + 0, y + 1)).b * 2.f
	+ RT::Color(_scene->image.getPixel(x + 1, y + 1)).b * 1.f;

      sobel[x + y * _scene->image.getSize().x] = sqrt(h_r * h_r + h_g * h_g + h_b * h_b + v_r * v_r + v_g * v_g + v_b * v_b);

      sobel_min = sobel_min < sobel[x + y * _scene->image.getSize().x] ? sobel_min : sobel[x + y * _scene->image.getSize().x];
      sobel_max = sobel_max > sobel[x + y * _scene->image.getSize().x] ? sobel_max : sobel[x + y * _scene->image.getSize().x];
    }

  // If no edge detected, cancel
  if (sobel_max - sobel_min == 0)
    return;

  // Set antialiasing to maximum for borders
  for (unsigned int i = 0; i < _antialiasing.size(); i++)
    _antialiasing[i] = RT::Config::PostAntiAliasing;

  // Apply sobel to antialiasing image
  for (int a = RT::Config::PostAntiAliasing; a >= 0; a--)
    for (unsigned int x = 1; x < _scene->image.getSize().x - 1; x++)
      for (unsigned int y = 1; y < _scene->image.getSize().y - 1; y++)
	if ((sobel[x + y * _scene->image.getSize().x] - sobel_min) / (sobel_max - sobel_min) < pow(1.f / 6.4f, RT::Config::PostAntiAliasing - a))
	  _antialiasing[x + y * _scene->image.getSize().x] = a;
}

void	RT::RenderRaytracer::render()
{
  while (active())
  {
    unsigned int  r = (unsigned int)Math::Random::rand((double)_grid.size());
    unsigned int  z = (unsigned int)_grid.size();

    // Find a zone to render
    for (unsigned int a = RT::Config::BlockSize; a > 0 && z == _grid.size(); a /= 2)
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
  unsigned int	x, y;

  _grid[zone] = RT::Config::BlockSize + 1;

  // Calcul zone coordinates (x, y)
  x = zone % (_scene->image.getSize().x / RT::Config::BlockSize + (_scene->image.getSize().x % RT::Config::BlockSize ? 1 : 0)) * RT::Config::BlockSize;
  y = zone / (_scene->image.getSize().x / RT::Config::BlockSize + (_scene->image.getSize().x % RT::Config::BlockSize ? 1 : 0)) * RT::Config::BlockSize;

  // Render zone
  for (unsigned int a = 0; a < RT::Config::BlockSize; a += size)
    for (unsigned int b = 0; b < RT::Config::BlockSize; b += size)
      if (x + a < _scene->image.getSize().x && y + b < _scene->image.getSize().y)
      {
	if (_status == First)
	{
	  if ((size == RT::Config::BlockSize || a % (size * 2) != 0 || b % (size * 2) != 0))
	  {
	    RT::Color clr = renderAntialiasing(x + a, y + b, RT::Config::AntiAliasing);

	    _progress++;
	    for (unsigned int c = 0; c < size; c++)
	      for (unsigned int d = 0; d < size; d++)
		if (x + a + c < _scene->image.getSize().x && y + b + d < _scene->image.getSize().y)
		  _scene->image.setPixel(x + a + c, y + b + d, clr.sfml());
	  }
	}
	else
	{
	  if (_antialiasing[(x + a) + (y + b) * _scene->image.getSize().x] > 0)
	  {
	    _scene->image.setPixel(x + a, y + b, renderAntialiasing(x + a, y + b, RT::Config::AntiAliasing + _antialiasing[(x + a) + (y + b) * _scene->image.getSize().x]).sfml());
	    _progress++;
	  }
	}
      }

  _grid[zone] = size / 2;
}

RT::Color RT::RenderRaytracer::renderAntialiasing(unsigned int x, unsigned int y, unsigned int antialiasing) const
{
  RT::Color	clr;

  // Divide a pixel to RT::Config::AntiAliasing² sub-pixels to avoid aliasing
  for (unsigned int a = 0; a < antialiasing; a++)
    for (unsigned int b = 0; b < antialiasing; b++)
    {
      Math::Ray	ray;

      // Calculate sub-pixel ray
      ray.px() = 0;
      ray.py() = 0;
      ray.pz() = 0;
      ray.dx() = _scene->image.getSize().x;
      ray.dy() = _scene->image.getSize().x / 2.f - x + (2.f * a + 1) / (2.f * antialiasing);
      ray.dz() = _scene->image.getSize().y / 2.f - y + (2.f * b + 1) / (2.f * antialiasing);

      // Sum rendered color
      clr += renderAnaglyph3D(ray.normalize());
    }

  // Return average color
  return clr / (antialiasing * antialiasing);
}

RT::Color RT::RenderRaytracer::renderAnaglyph3D(Math::Ray const & ray) const
{
  RT::Color	      clr_left, clr_right;
  Math::Matrix<4, 4>  cam_left(_scene->camera), cam_right(_scene->camera);
  double	      angle;

  // If no offset between eyes, do not render anaglyph
  if (RT::Config::Anaglyph3D::Offset == 0)
    return renderDephOfField((_scene->camera * ray).normalize());

  // Calculate focus angle
  angle = Math::Pi / 2.f - atan(RT::Config::Anaglyph3D::Focal / ((RT::Config::Anaglyph3D::Offset == 0 ? 1 : RT::Config::Anaglyph3D::Offset) / 2.f));

  // Calculate left/right eye ray accoding offset and focus angle
  cam_left = cam_left * Math::Matrix<4, 4>::translation(0, +RT::Config::Anaglyph3D::Offset / 2.f, 0) * Math::Matrix<4, 4>::rotation(0, 0, -Math::Utils::RadToDeg(angle));
  cam_right = cam_right * Math::Matrix<4, 4>::translation(0, -RT::Config::Anaglyph3D::Offset / 2.f, 0) * Math::Matrix<4, 4>::rotation(0, 0, +Math::Utils::RadToDeg(angle));

  // Render left/right colors
  clr_left = renderDephOfField((cam_left * ray).normalize());
  clr_right = renderDephOfField((cam_right * ray).normalize());

  // Return color using masks RT::Config::Anaglyph3D::MaskLeft/Right
  return (clr_left * RT::Config::Anaglyph3D::MaskLeft) + (clr_right * RT::Config::Anaglyph3D::MaskRight);
}

RT::Color RT::RenderRaytracer::renderDephOfField(Math::Ray const & ray) const
{
  unsigned int	      nb_ray;
  Math::Matrix<4, 4>  rot = Math::Matrix<4, 4>::rotation(Math::Random::rand(360.f), Math::Random::rand(360.f), Math::Random::rand(360.f));
  Math::Ray	      pt = Math::Matrix<4, 4>::translation(ray.dx() * RT::Config::DephOfField::Focal, ray.dy() * RT::Config::DephOfField::Focal, ray.dz() * RT::Config::DephOfField::Focal) * ray;
  RT::Color	      clr;

  // If no deph of field, just return rendered ray
  if (RT::Config::DephOfField::Quality <= 1 || RT::Config::DephOfField::Focal == 0)
    return renderRay(ray);

  // Generate multiples ray inside aperture to simulate deph of field
  nb_ray = 0;
  for (double a = Math::Random::rand(RT::Config::DephOfField::Aperture / RT::Config::DephOfField::Quality); a < RT::Config::DephOfField::Aperture; a += RT::Config::DephOfField::Aperture / RT::Config::DephOfField::Quality)
    for (double b = Math::Random::rand(Math::Pi / (RT::Config::DephOfField::Quality * 2)) - Math::Pi / 2.f; b < Math::Pi / 2.f; b += Math::Pi / RT::Config::DephOfField::Quality)
    {
      int	nb = (int)(cos(b) * RT::Config::DephOfField::Quality * 2.f * (a / RT::Config::DephOfField::Aperture));
      if (nb > 0)
	for (double c = Math::Random::rand(Math::Pi * 2.f / nb); c < Math::Pi * 2.f; c += Math::Pi * 2.f / nb)
	{
	  Math::Ray	deph;

	  // Calcul point position in aperture sphere
	  deph.px() = sin(b) * a;
	  deph.py() = cos(c) * cos(b) * a;
	  deph.pz() = sin(c) * cos(b) * a;

	  // Randomly rotate point to smooth rendered image
	  deph = rot * deph;

	  // Calculate ray
	  deph.px() = deph.px() + ray.px();
	  deph.py() = deph.py() + ray.py();
	  deph.pz() = deph.pz() + ray.pz();
	  deph.dx() = pt.px() - deph.px();
	  deph.dy() = pt.py() - deph.py();
	  deph.dz() = pt.pz() - deph.pz();

	  // Sum rendered colors
	  clr += renderRay(deph.normalize());

	  nb_ray++;
	}
    }

  // Return average color
  return clr / nb_ray;
}

RT::Color RT::RenderRaytracer::renderRay(Math::Ray const & ray, unsigned int recursivite) const
{
  if (recursivite > RT::Config::MaxRecursivite)
    return RT::Color(0.f);

  std::list<RT::Intersection>	intersect;

  // Render intersections list with CSG tree
  if (_scene->tree)
    intersect = _scene->tree->render(ray);

  // Drop intersection behind camera
  while (!intersect.empty() && intersect.front().distance < 0)
    intersect.pop_front();

  // If no intersection, return background color (black)
  if (!intersect.empty())
  {
    RT::Color	reflection, transparency, light;

    // Calculate transparency, reflection and light
    reflection = renderReflection(ray, intersect.front(), recursivite);
    transparency = renderTransparency(ray, intersect.front(), recursivite);
    light = renderLight(ray, intersect.front(), recursivite);

    // Return color applying light modifiers
    return reflection + transparency + light;
  }
  else
    return RT::Color(0.f);
}

RT::Color RT::RenderRaytracer::renderReflection(Math::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
{
  if (intersection.material.reflection == 0.f ||
    intersection.material.transparency == 1.f)
    return RT::Color(0.f);

  Math::Ray new_ray, normal;
  double    k;

  // Reverse normal if necessary
  normal = intersection.normal;
  if (Math::Ray::cos(ray, intersection.normal) > 0)
    normal.d() = Math::Matrix<4, 4>::scale(-1.f) * intersection.normal.d();

  k = -(normal.dx() * (normal.px() - ray.px()) + normal.dy() * (normal.py() - ray.py()) + normal.dz() * (normal.pz() - ray.pz())) / (normal.dx() * normal.dx() + normal.dy() * normal.dy() + normal.dz() * normal.dz());

  new_ray.px() = normal.px() + normal.dx() * Math::Shift;
  new_ray.py() = normal.py() + normal.dy() * Math::Shift;
  new_ray.pz() = normal.pz() + normal.dz() * Math::Shift;
  new_ray.dx() = ray.px() + 2.f * (normal.px() + normal.dx() * k - ray.px()) - normal.px();
  new_ray.dy() = ray.py() + 2.f * (normal.py() + normal.dy() * k - ray.py()) - normal.py();
  new_ray.dz() = ray.pz() + 2.f * (normal.pz() + normal.dz() * k - ray.pz()) - normal.pz();

  return renderRay(new_ray, recursivite + 1) * intersection.material.color * intersection.material.reflection * (1.f - intersection.material.transparency);
}

RT::Color RT::RenderRaytracer::renderTransparency(Math::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
{
  if (intersection.material.transparency == 0.f)
    return RT::Color(0.f);

  Math::Ray normal;
  double    refraction;
  double  a, b, c, l;

  // Inverse normal and refraction if necessary
  normal = intersection.normal;
  refraction = intersection.material.refraction;
  if (Math::Ray::cos(ray, intersection.normal) > 0)
  {
    normal.d() = Math::Matrix<4, 4>::scale(-1.f) * intersection.normal.d();
    refraction = 1.f / refraction;
  }

  // Using sphere technique to calculate refracted ray
  l = sqrt(ray.dx() * ray.dx() + ray.dy() * ray.dy() + ray.dz() * ray.dz());
  a = normal.dx() * normal.dx() + normal.dy() * normal.dy() + normal.dz() * normal.dz();
  b = 2.f * (ray.dx() * normal.dx() + ray.dy() * normal.dy() + ray.dz() * normal.dz());
  c = ray.dx() * ray.dx() + ray.dy() * ray.dy() + ray.dz() * ray.dz() - (refraction * l) * (refraction * l);

  std::vector<double> result = Math::Utils::solve(a, b, c);

  // Reflection if invalid refraction
  Math::Ray new_ray;
  if (result.empty())
  {
    double  k = -(normal.dx() * (normal.px() - ray.px()) + normal.dy() * (normal.py() - ray.py()) + normal.dz() * (normal.pz() - ray.pz())) / (normal.dx() * normal.dx() + normal.dy() * normal.dy() + normal.dz() * normal.dz());

    new_ray.px() = normal.px() + normal.dx() * Math::Shift;
    new_ray.py() = normal.py() + normal.dy() * Math::Shift;
    new_ray.pz() = normal.pz() + normal.dz() * Math::Shift;
    new_ray.dx() = ray.px() + 2.f * (normal.px() + normal.dx() * k - ray.px()) - normal.px();
    new_ray.dy() = ray.py() + 2.f * (normal.py() + normal.dy() * k - ray.py()) - normal.py();
    new_ray.dz() = ray.pz() + 2.f * (normal.pz() + normal.dz() * k - ray.pz()) - normal.pz();
  }
  else
  {
    new_ray.px() = normal.px() - normal.dx() * Math::Shift;
    new_ray.py() = normal.py() - normal.dy() * Math::Shift;
    new_ray.pz() = normal.pz() - normal.dz() * Math::Shift;
    new_ray.dx() = ray.dx() + normal.dx() * result.front();
    new_ray.dy() = ray.dy() + normal.dy() * result.front();
    new_ray.dz() = ray.dz() + normal.dz() * result.front();
  }

  return renderRay(new_ray, recursivite) * intersection.material.color * intersection.material.transparency;
}

RT::Color RT::RenderRaytracer::renderLight(Math::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
{
  RT::Color light;

  for (std::list<RT::AbstractLight const *>::const_iterator it = _scene->light.begin(); it != _scene->light.end(); it++)
    light += (*it)->render(_scene->tree, ray, intersection.normal, intersection.material);

  return light;
}

double	  RT::RenderRaytracer::progress() const
{
  if (_status == First)
  {
    unsigned int  r = 0;

    for (unsigned int a = 0; a < _grid.size(); a++)
      if (_grid[a] == 0)
	r++;

    if (r == _grid.size())
      return 0.5f;
    else
      return 0.5f * _progress / (_scene->image.getSize().x * _scene->image.getSize().y * RT::Config::AntiAliasing * RT::Config::AntiAliasing);
  }
  else
  {
    unsigned int  r = 0;

    for (unsigned int a = 0; a < _grid.size(); a++)
      if (_grid[a] == 0)
	r++;
  
    if (r == _grid.size())
      return 1.f;
    else
    {
      double  n = 0;

      for (unsigned int a = 0; a < _antialiasing.size(); a++)
	if (_antialiasing[a] != 0)
	  n++;

      return 0.5f + 0.5f * (double)_progress / (double)n;
    }
  }
}