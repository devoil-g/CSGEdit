#include <iomanip>
#include <iostream>
#include <functional>
#include <thread>

#include <SFML/System/Thread.hpp>
#include <SFML/System/Sleep.hpp>

#include "Raytracer.hpp"
#include "Exception.hpp"
#include "Config.hpp"

#include "CsgNode.hpp"
#include "MaterialNode.hpp"
#include "MeshNode.hpp"

#include "BoxLeaf.hpp"
#include "ConeLeaf.hpp"
#include "SphereLeaf.hpp"
#include "TangleLeaf.hpp"
#include "TorusLeaf.hpp"

#include "DirectionalLight.hpp"
#include "OcclusionLight.hpp"
#include "PointLight.hpp"

RT::Raytracer::Raytracer()
  : _image(), _lock(), _tree(nullptr), _camera(Math::Matrix<4, 4>::translation(-540, 52, 436) * Math::Matrix<4, 4>::rotation(0, 39.8, -7.2)), _grid(), _thread(), _continue(false)
{
  // Set image to window size
  _image.create(RT::Config::WindowWidth, RT::Config::WindowHeight);
}

RT::Raytracer::~Raytracer()
{
  delete _tree;

  while (!_light.empty())
  {
    delete _light.front();
    _light.pop_front();
  }
}

bool	RT::Raytracer::load(std::string const &)
{
  // NOTE: Implement a parser here

  _lock.lock();

  // Stop working threads
  stop();

  // Reset image pixels
  for (unsigned int y = 0; y < RT::Config::WindowHeight; y++)
    for (unsigned int x = 0; x < RT::Config::WindowWidth; x++)
      _image.setPixel(x, y, RT::Color(0.084f, 0.084f, 0.084f).sfml());

  _grid.resize(0);
  _method = nullptr;

  RT::AbstractNode *  node = new RT::CsgNode(RT::CsgNode::Union);
  RT::AbstractNode *  material_node;
  RT::AbstractNode *  material_node2;
  RT::Material	      material;

  material.color = RT::Color(255.f / 255.f, 255.f / 255.f, 225.f / 255.f);
  material.shine = 8.4f;
  material_node = new RT::MaterialNode(material);
  material_node->push(new RT::BoxLeaf(Math::Matrix<4, 4>::translation(0, 0, -230), 420, true));
  for (int x = 0; x < 4; x++)
    for (int y = 0; y < 4; y++)
      material_node->push(new RT::BoxLeaf(Math::Matrix<4, 4>::translation(x * 100 - 150, y * 100 - 150, x * 20 - 40), 80, true));
  node->push(material_node);

  material.color = RT::Color(225.f / 255.f, 225.f / 255.f, 225.f / 255.f);
  material_node = new RT::MaterialNode(material);

  // Line 1
  material_node->push(new RT::SphereLeaf(Math::Matrix<4, 4>::translation(-150, +150, 28 + 1), 28));
  material_node->push(new RT::ConeLeaf(Math::Matrix<4, 4>::translation(-150, +50, 28 + 1), 24, 56, true));
  material_node->push(new RT::ConeLeaf(Math::Matrix<4, 4>::translation(-150, -50, 28 + 1), 32, -28, 56, true));
  material_node->push(new RT::BoxLeaf(Math::Matrix<4, 4>::translation(-150, -150, 22 + 1) * Math::Matrix<4, 4>::rotation(0, 0, -10), 44, true));
  
  // Line 2
  material_node->push(new RT::MeshNode(Math::Matrix<4, 4>::translation(-72, +150, 19 + 1) * Math::Matrix<4, 4>::scale(0.64f, 0.64f, 0.64f) * Math::Matrix<4, 4>::rotation(0, 0, -30), "C:/project/CSG-Raytracer/assets/models/bunny.stl"));
  material_node->push(new RT::TangleLeaf(Math::Matrix<4, 4>::translation(-50, +50, 39 + 1) * Math::Matrix<4, 4>::scale(10.f, 10.f, 10.f) * Math::Matrix<4, 4>::rotation(0, 0, +30), 11.8f));
  material_node->push(new RT::TorusLeaf(Math::Matrix<4, 4>::translation(-50, -50, 43 + 1) * Math::Matrix<4, 4>::rotation(45, 0, -45), 22, 8));

  // Line 3
  material.color = RT::Color(255.f / 255.f, 255.f / 255.f, 255.f / 255.f);
  material.refraction = 1.f;
  material.reflection = 0.72f;
  material.transparency = 0.f;
  material_node2 = new RT::MaterialNode(material);
  material_node2->push(new RT::SphereLeaf(Math::Matrix<4, 4>::translation(+50, +150, 68 + 1), 28));
  material_node->push(material_node2);

  material.color = RT::Color(255.f / 225.f);
  material.reflection = 0.f;
  material.refraction = 1.21f;
  material.transparency = 0.92f;
  material_node2 = new RT::MaterialNode(material);
  material_node2->push(new RT::SphereLeaf(Math::Matrix<4, 4>::translation(+50, +50, 68 + 1), 28));
  material_node->push(material_node2);

  material.transparency = 0.72f;
  material.refraction = 1.f;
  material_node2 = new RT::MaterialNode(material);
  material_node2->push(new RT::BoxLeaf(Math::Matrix<4, 4>::translation(+50, -50, 64 + 1), 48, true));
  material_node->push(material_node2);

  material.color = RT::Color(255.f / 255.f, 105.f / 255.f, 105.f / 255.f);
  material.transparency = 0.f;
  material_node2 = new RT::MaterialNode(material);
  material_node2->push(new RT::BoxLeaf(Math::Matrix<4, 4>::translation(+50, -130, 66 + 1), 52, 12, 52, true));
  material_node->push(material_node2);

  material.color = RT::Color(105.f / 255.f, 255.f / 255.f, 105.f / 255.f);
  material_node2 = new RT::MaterialNode(material);
  material_node2->push(new RT::BoxLeaf(Math::Matrix<4, 4>::translation(+50, -150, 66 + 1), 52, 12, 52, true));
  material_node->push(material_node2);

  material.color = RT::Color(105.f / 255.f, 105.f / 255.f, 255.f / 255.f);
  material_node2 = new RT::MaterialNode(material);
  material_node2->push(new RT::BoxLeaf(Math::Matrix<4, 4>::translation(+50, -170, 66 + 1), 52, 12, 52, true));
  material_node->push(material_node2);

  // Line 4
  material_node->push(RT::createUnion(new RT::BoxLeaf(Math::Matrix<4, 4>::translation(+150, +150, 84 + 1), 48, true), new RT::SphereLeaf(Math::Matrix<4, 4>::translation(+150, +150, 84 + 1), 30)));
  material_node->push(RT::createDifference(new RT::SphereLeaf(Math::Matrix<4, 4>::translation(+150, +50, 84 + 1), 30), new RT::BoxLeaf(Math::Matrix<4, 4>::translation(+150, +50, 84 + 1), 48, true)));
  material_node->push(RT::createDifference(new RT::BoxLeaf(Math::Matrix<4, 4>::translation(+150, -50, 84 + 1), 48, true), new RT::SphereLeaf(Math::Matrix<4, 4>::translation(+150, -50, 84 + 1), 30)));
  material_node->push(RT::createIntersection(new RT::BoxLeaf(Math::Matrix<4, 4>::translation(+150, -150, 84 + 1), 48, true), new RT::SphereLeaf(Math::Matrix<4, 4>::translation(+150, -150, 84 + 1), 30)));

  node->push(material_node);
  
  // Set _tree to CSG tree
  _tree = node;

  _light.push_back(new RT::OcclusionLight());
  _light.push_back(new RT::DirectionalLight(Math::Matrix<4, 4>::rotation(0, 60, 30), RT::Color(1.f), 4.2f));
  //_light.push_back(new RT::PointLight(Math::Matrix<4, 4>::translation(0,0,+128), RT::Color(1.f), 21.42f, 0.f));

  _lock.unlock();
  return true;
}

void  RT::Raytracer::preview()
{
  _lock.lock();

  // Stop working threads
  stop();

  // Reset image pixels
  for (unsigned int y = 0; y < RT::Config::WindowHeight; y++)
    for (unsigned int x = 0; x < RT::Config::WindowWidth; x++)
      _image.setPixel(x, y, RT::Color(0.084f, 0.084f, 0.084f).sfml());

  // Reset zone grid
  _grid.resize(((RT::Config::WindowWidth / 2) / RT::Config::ThreadSize + ((RT::Config::WindowWidth / 2) % RT::Config::ThreadSize ? 1 : 0)) * ((RT::Config::WindowHeight / 2) / RT::Config::ThreadSize + ((RT::Config::WindowHeight / 2) % RT::Config::ThreadSize ? 1 : 0)));
  for (unsigned int i = 0; i < _grid.size(); i++)
    _grid[i] = Wait;

  // Set method pointer to preview
  _method = &RT::Raytracer::preview;

  _lock.unlock();
}

void  RT::Raytracer::render()
{
  _lock.lock();

  // Stop working threads
  stop();

  // Reset image pixels
  for (unsigned int y = 0; y < RT::Config::WindowHeight; y++)
    for (unsigned int x = 0; x < RT::Config::WindowWidth; x++)
      if (x % RT::Config::ThreadSize == 0 || y % RT::Config::ThreadSize == 0)
	_image.setPixel(x, y, RT::Color(0.1f, 0.1f, 0.1f).sfml());
      else
	_image.setPixel(x, y, RT::Color(0.084f, 0.084f, 0.084f).sfml());

  // Reset zone grid
  _grid.resize((RT::Config::WindowWidth / RT::Config::ThreadSize + (RT::Config::WindowWidth % RT::Config::ThreadSize ? 1 : 0)) * (RT::Config::WindowHeight / RT::Config::ThreadSize + (RT::Config::WindowHeight % RT::Config::ThreadSize ? 1 : 0)));
  for (unsigned int i = 0; i < _grid.size(); i++)
    _grid[i] = Wait;

  // Set method pointer to render
  _method = &RT::Raytracer::render;

  _lock.unlock();
}

void  RT::Raytracer::start()
{
  _lock.lock();

  // Stop working threads
  stop();
  _continue = true;

  // Create and launch rendering threads
  for (unsigned int i = 0; i < RT::Config::ThreadNumber; i++)
  {
    _thread.push_back(new sf::Thread(&RT::Raytracer::routine, this));
    _thread.back()->launch();
  }

  _lock.unlock();
}

void  RT::Raytracer::stop()
{
  _lock.lock();

  _continue = false;

  // Stop and delete threads
  while (!_thread.empty())
  {
    _thread.back()->wait();
    delete _thread.back();
    _thread.pop_back();
  }

  // Set all zone not fully rendered to "Wait" status
  for (unsigned int i = 0; i < _grid.size(); i++)
    if (_grid[i] == Progress)
      _grid[i] = Wait;

  _lock.unlock();
}

double	RT::Raytracer::progress()
{
  _lock.lock();

  // Stop if no rendering commissionned
  if (_grid.empty())
  {
    _lock.unlock();
    return 0;
  }

  // Count grid zone completed
  unsigned int	count = 0;
  for (unsigned int i = 0; i < _grid.size(); i++)
    if (_grid[i] == Done)
      count++;

  // Calcul result
  double result = (double)count / (double)_grid.size();

  _lock.unlock();
  return result;
}

Math::Matrix<4, 4> const &  RT::Raytracer::camera() const
{
  return _camera;
}

sf::Image const & RT::Raytracer::image() const
{
  return _image;
}

void  RT::Raytracer::routine()
{
  // Stop if invalid grid or routine method
  if (_grid.size() == 0 || _method == nullptr)
    return;

  while (_continue)
  {
    unsigned int  r = (unsigned int)Math::Random::rand((double)_grid.size());
    unsigned int  z = (unsigned int)_grid.size();

    // Find a zone to render
    for (unsigned int a = 0; a < _grid.size() && z == _grid.size(); a++)
      if (_grid[(r + a) % _grid.size()] == Wait)
	z = (r + a) % _grid.size();
    for (unsigned int a = 0; a < _grid.size() && z == _grid.size(); a++)
      if (_grid[(r + a) % _grid.size()] == Progress)
	z = (r + a) % _grid.size();

    // Return if no zone to render
    if (z == _grid.size())
      return;
    else
    {
      _grid[z] = Progress;
      (this->*_method)(z);
      _grid[z] = Done;
    }
  }
}

void  RT::Raytracer::preview(unsigned int zone)
{
  unsigned int	x, y;

  // Calcul zone coordinates (x, y)
  x = zone % ((RT::Config::WindowWidth / 2) / RT::Config::ThreadSize + ((RT::Config::WindowWidth / 2) % RT::Config::ThreadSize ? 1 : 0)) * RT::Config::ThreadSize;
  y = zone / ((RT::Config::WindowWidth / 2) / RT::Config::ThreadSize + ((RT::Config::WindowWidth / 2) % RT::Config::ThreadSize ? 1 : 0)) * RT::Config::ThreadSize;

  // Render zone
  for (unsigned int a = 0; a < RT::Config::ThreadSize && _continue; a++)
    for (unsigned int b = 0; b < RT::Config::ThreadSize && _continue; b++)
      if (x + a < RT::Config::WindowWidth / 2 && y + b < RT::Config::WindowHeight / 2)
	_image.setPixel(RT::Config::WindowWidth / 4 + x + a, RT::Config::WindowHeight / 4 + y + b, preview(x + a, y + b).sfml());
}

RT::Color	RT::Raytracer::preview(unsigned int x, unsigned int y) const
{
  std::list<RT::Intersection>	intersect;
  Math::Ray			ray;

  // Calcul ray according to (x, y) coordinates
  ray.px() = 0;
  ray.py() = 0;
  ray.pz() = 0;
  ray.dx() = (double)RT::Config::WindowWidth;
  ray.dy() = (double)RT::Config::WindowWidth / 2 - x * 2 + 0.5f;
  ray.dz() = (double)RT::Config::WindowHeight / 2 - y * 2 + 0.5f;
  ray = (_camera * ray).normalize();

  // Render intersections using ray
  if (_tree)
    intersect = _tree->render(ray);

  // Delete back intersections
  while (!intersect.empty() && intersect.front().distance < 0)
    intersect.pop_front();

  // Calcul intersection color if it exist, otherwise return black
  if (!intersect.empty())
  {
    RT::Color light;

    for (std::list<RT::AbstractLight const *>::const_iterator it = _light.begin(); it != _light.end(); it++)
      light += (*it)->preview(_tree, ray, intersect.front().normal, intersect.front().material);

    return light;
  }
  else
    return RT::Color(0.f);
}

void  RT::Raytracer::render(unsigned int zone)
{
  unsigned int	x, y;

  // Calcul zone coordinates (x, y)
  x = zone % (RT::Config::WindowWidth / RT::Config::ThreadSize + (RT::Config::WindowWidth % RT::Config::ThreadSize ? 1 : 0)) * RT::Config::ThreadSize;
  y = zone / (RT::Config::WindowWidth / RT::Config::ThreadSize + (RT::Config::WindowWidth % RT::Config::ThreadSize ? 1 : 0)) * RT::Config::ThreadSize;

  // Render zone
  for (unsigned int a = 0; a < RT::Config::ThreadSize && _continue; a++)
    for (unsigned int b = 0; b < RT::Config::ThreadSize && _continue; b++)
      if (x + a < RT::Config::WindowWidth && y + b < RT::Config::WindowHeight)
	_image.setPixel(x + a, y + b, render(x + a, y + b).sfml());
}

RT::Color RT::Raytracer::render(unsigned int x, unsigned int y) const
{
  return renderAntialiasing(x, y);
}

RT::Color RT::Raytracer::renderAntialiasing(unsigned int x, unsigned int y) const
{
  RT::Color	clr;

  // Divide a pixel to RT::Config::AntiAliasing² sub-pixels to avoid aliasing
  for (unsigned int a = 0; a < RT::Config::AntiAliasing; a++)
    for (unsigned int b = 0; b < RT::Config::AntiAliasing; b++)
    {
      Math::Ray	ray;

      // Calculate sub-pixel ray
      ray.px() = 0;
      ray.py() = 0;
      ray.pz() = 0;
      ray.dx() = RT::Config::WindowWidth;
      ray.dy() = RT::Config::WindowWidth / 2.f - x + (2.f * a + 1) / (2.f * RT::Config::AntiAliasing);
      ray.dz() = RT::Config::WindowHeight / 2.f - y + (2.f * b + 1) / (2.f * RT::Config::AntiAliasing);

      // Sum rendered color
      clr += renderAnaglyph3D(ray.normalize());
    }

  // Return average color
  return clr / (RT::Config::AntiAliasing * RT::Config::AntiAliasing);
}

RT::Color RT::Raytracer::renderAnaglyph3D(Math::Ray const & ray) const
{
  RT::Color	      clr_left, clr_right;
  Math::Matrix<4, 4>  cam_left(_camera), cam_right(_camera);
  double	      angle;

  // If no offset between eyes, do not render anaglyph
  if (RT::Config::Anaglyph3D::Offset == 0)
    return renderDephOfField((_camera * ray).normalize());

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

RT::Color RT::Raytracer::renderDephOfField(Math::Ray const & ray) const
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

RT::Color RT::Raytracer::renderRay(Math::Ray const & ray, unsigned int recursivite) const
{
  if (recursivite > RT::Config::MaxRecursivite)
    return RT::Color(0.f);

  std::list<RT::Intersection>	intersect;

  // Render intersections list with CSG tree
  if (_tree)
    intersect = _tree->render(ray);

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

RT::Color RT::Raytracer::renderReflection(Math::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
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

RT::Color RT::Raytracer::renderTransparency(Math::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
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

RT::Color RT::Raytracer::renderLight(Math::Ray const & ray, RT::Intersection const & intersection, unsigned int recursivite) const
{
  RT::Color light;

  for (std::list<RT::AbstractLight const *>::const_iterator it = _light.begin(); it != _light.end(); it++)
    light += (*it)->render(_tree, ray, intersection.normal, intersection.material);

  return light;
}
