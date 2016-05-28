#include <stdexcept>
#include <fstream>

#include "MeshCsgNode.hpp"
#include "TriangleCsgLeaf.hpp"

std::map<std::string, void(RT::MeshCsgNode::*)(std::string const &)> const	RT::MeshCsgNode::_extension =
{
  { ".stl", &RT::MeshCsgNode::loadStl }
};

RT::MeshCsgNode::MeshCsgNode()
  : _x(0.f), _y(0.f), _z(0.f), _r(0.f)
{}

RT::MeshCsgNode::MeshCsgNode(std::string const & path)
  : _x(0.f), _y(0.f), _z(0.f), _r(0.f)
{
  // Load file according to extension
  if (extension(path.substr(path.rfind('.'))))
    (this->*_extension.find(path.substr(path.rfind('.')))->second)(path);
  else
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  bound();
}

RT::MeshCsgNode::~MeshCsgNode()
{}

std::list<RT::Intersection>	RT::MeshCsgNode::render(RT::Ray const & ray, unsigned int deph) const
{
  // Stop if no intersection with bounding sphere
  if (std::pow(2.f * ((ray.p().x() - _x) * ray.d().x() + (ray.p().y() - _y) * ray.d().y() + (ray.p().z() - _z) * ray.d().z()), 2) - 4.f * (std::pow(ray.d().x(), 2) + std::pow(ray.d().y(), 2) + std::pow(ray.d().z(), 2)) * (std::pow(ray.p().x() - _x, 2) + std::pow(ray.p().y() - _y, 2) + std::pow(ray.p().z() - _z, 2) - std::pow(_r, 2)) < 0.f)
    return std::list<RT::Intersection>();

  std::list<RT::Intersection>	result;

  // Iterate through triangles to get intersections
  for (RT::AbstractCsgTree const * it : children())
    result.merge(it->render(ray, deph));

  return result;
}

void				RT::MeshCsgNode::loadStl(std::string const & path)
{
  std::ifstream			file(path, std::ios::in | std::ios::binary);
  unsigned int			n_tri;
  char				header[80];

  // DO NOT SUPPORT ASCII STL

  if (!file.good())
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  // Render header (80 bytes)
  file.read(header, 80);

  // Read number of triangles (4 bytes)
  file.read((char *)&n_tri, sizeof(unsigned int));
  if (n_tri > 16384)
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  // Get all triangles (50 bytes each)
  for (unsigned int i = 0; i < n_tri; i++)
  {
    float	facet[16];

    file.read((char *)facet, 50);
    push(new TriangleCsgLeaf(std::tuple<double, double, double>(facet[3], facet[4], facet[5]), std::tuple<double, double, double>(facet[6], facet[7], facet[8]), std::tuple<double, double, double>(facet[9], facet[10], facet[11])));
  }
}

void				RT::MeshCsgNode::push(RT::AbstractCsgTree * node)
{
  if (dynamic_cast<RT::TriangleCsgLeaf *>(node))
    RT::AbstractCsgNode::push(node);
  else
    throw std::runtime_error((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  // Update bounding sphere if new points outside previous sphere
  for (std::tuple<double, double, double> & pt : dynamic_cast<RT::TriangleCsgLeaf *>(node)->points())
    if (std::pow(std::get<0>(pt) - _x, 2) + std::pow(std::get<1>(pt) - _y, 2) + std::pow(std::get<2>(pt) - _z, 2) > std::pow(_r, 2))
    {
      bound();
      return;
    }
}

bool				RT::MeshCsgNode::extension(std::string const & ext)
{
  if (_extension.find(ext) != _extension.end())
    return true;
  else
    return false;
}

void				RT::MeshCsgNode::bound()
{
  // Retrieve all points from children
  std::list<std::tuple<double, double, double>>	pts;

  for (RT::AbstractCsgTree const * it : children())
    for (std::tuple<double, double, double> pt : dynamic_cast<RT::TriangleCsgLeaf const *>(it)->points())
      pts.push_back(pt);
  
  // Remove duplicates
  pts.sort();
  pts.unique();

  RT::MeshCsgNode::BoundingSphere	ball(pts);

  _x = std::get<0>(ball.center);
  _y = std::get<1>(ball.center);
  _z = std::get<2>(ball.center);
  _r = std::sqrt(ball.radius);
}

RT::MeshCsgNode::BoundingSphere::BoundingSphere(std::list<std::tuple<double, double, double>> points)
{
  _q0 = std::tuple<double, double, double>(0.f, 0.f, 0.f);
  _fsize = 0;
  _ssize = 0;

  for (unsigned int i = 0; i < 4; i++)
  {
    _a[i] = std::tuple<double, double, double>(0.f, 0.f, 0.f);
    _c[i] = std::tuple<double, double, double>(0.f, 0.f, 0.f);
    _v[i] = std::tuple<double, double, double>(0.f, 0.f, 0.f);
    _f[i] = 0;
    _z[i] = 0;
    _s[i] = 0;
  }

  center = std::tuple<double, double, double>(0.f, 0.f, 0.f);
  radius = -1.f;

  for (double old_sqr_r = -2.f; old_sqr_r < radius;)
  {
    old_sqr_r = radius;

    std::tuple<double, double, double> pivot = points.front();
    double  max_e = 0;

    for (std::tuple<double, double, double> & pt : points)
    {
      double e = std::pow(std::get<0>(pt) - std::get<0>(center), 2) + std::pow(std::get<1>(pt) - std::get<1>(center), 2) + std::pow(std::get<2>(pt) - std::get<2>(center), 2) - radius;

      if (e > max_e)
      {
	max_e = e;
	pivot = pt;
      }
    }

    if (max_e > 0 && std::find(_support.begin(), _support.end(), pivot) == _support.end() && push(pivot))
    {
      mtf_mb(_support);
      --_fsize;

      _support.push_front(pivot);
      if (_support.size() > 4)
	_support.pop_back();
    }
  }
}

void				RT::MeshCsgNode::BoundingSphere::mtf_mb(std::list<std::tuple<double, double, double>> list)
{
  _support.clear();

  if ((_fsize) == 4)
    return;

  for (std::list<std::tuple<double, double, double>>::iterator it = list.begin(); it != list.end(); it++)
    if (std::pow(std::get<0>(*it) - std::get<0>(center), 2) + std::pow(std::get<1>(*it) - std::get<1>(center), 2) + std::pow(std::get<2>(*it) - std::get<2>(center), 2) - radius > 0.f && push(*it))
    {
      mtf_mb(std::list<std::tuple<double, double, double>>(list.begin(), it));
      --_fsize;

      _support.push_front(*it);
    }
}

bool				RT::MeshCsgNode::BoundingSphere::push(std::tuple<double, double, double> pit)
{
  if (_fsize == 0)
  {
    _q0 = pit;
    _c[0] = pit;
    _s[0] = 0;
  }

  else
  {
    std::get<0>(_v[_fsize]) = std::get<0>(pit) - std::get<0>(_q0);
    std::get<1>(_v[_fsize]) = std::get<1>(pit) - std::get<1>(_q0);
    std::get<2>(_v[_fsize]) = std::get<2>(pit) - std::get<2>(_q0);

    if (1 < _fsize)
      std::get<1>(_a[_fsize]) = (std::get<0>(_v[1]) * std::get<0>(_v[_fsize]) + std::get<1>(_v[1]) * std::get<1>(_v[_fsize]) + std::get<2>(_v[1]) * std::get<2>(_v[_fsize])) * (2.f / _z[1]);
    if (2 < _fsize)
      std::get<2>(_a[_fsize]) = (std::get<0>(_v[2]) * std::get<0>(_v[_fsize]) + std::get<1>(_v[2]) * std::get<1>(_v[_fsize]) + std::get<2>(_v[2]) * std::get<2>(_v[_fsize])) * (2.f / _z[2]);

    if (1 < _fsize)
    {
      std::get<0>(_v[_fsize]) -= std::get<1>(_a[_fsize]) * std::get<0>(_v[1]);
      std::get<1>(_v[_fsize]) -= std::get<1>(_a[_fsize]) * std::get<1>(_v[1]);
      std::get<2>(_v[_fsize]) -= std::get<1>(_a[_fsize]) * std::get<2>(_v[1]);
    }
    if (2 < _fsize)
    {
      std::get<0>(_v[_fsize]) -= std::get<2>(_a[_fsize]) * std::get<0>(_v[2]);
      std::get<1>(_v[_fsize]) -= std::get<2>(_a[_fsize]) * std::get<1>(_v[2]);
      std::get<2>(_v[_fsize]) -= std::get<2>(_a[_fsize]) * std::get<2>(_v[2]);
    }

    _z[_fsize] = (std::pow(std::get<0>(_v[_fsize]), 2) + std::pow(std::get<1>(_v[_fsize]), 2) + std::pow(std::get<2>(_v[_fsize]), 2)) * 2.f;

    if (_z[_fsize] < std::pow(std::numeric_limits<double>::epsilon(), 2) * radius)
      return false;

    double e = std::pow(std::get<0>(pit) - std::get<0>(_c[_fsize - 1]), 2) + std::pow(std::get<1>(pit) - std::get<1>(_c[_fsize - 1]), 2) + std::pow(std::get<2>(pit) - std::get<2>(_c[_fsize - 1]), 2) - _s[_fsize - 1];
    _f[_fsize] = e / _z[_fsize];

    std::get<0>(_c[_fsize]) = std::get<0>(_c[_fsize - 1]) + _f[_fsize] * std::get<0>(_v[_fsize]);
    std::get<1>(_c[_fsize]) = std::get<1>(_c[_fsize - 1]) + _f[_fsize] * std::get<1>(_v[_fsize]);
    std::get<2>(_c[_fsize]) = std::get<2>(_c[_fsize - 1]) + _f[_fsize] * std::get<2>(_v[_fsize]);

    _s[_fsize] = _s[_fsize - 1] + e * _f[_fsize] / 2.f;
  }

  center = _c[_fsize];
  radius = _s[_fsize];
  _ssize = ++_fsize;

  return true;
}

size_t				RT::MeshCsgNode::build(std::vector<RT::OpenCL::Node> & nodes, std::vector<RT::OpenCL::Primitive> & primitives, Math::Matrix<4, 4> const & transformation, RT::Material const & material, unsigned int deph) const
{
  size_t			id = children().front()->build(nodes, primitives, transformation, material, deph);

  nodes.push_back(RT::OpenCL::Node());
  nodes.back().type = RT::OpenCL::Node::Type::TypePrimitive;
  nodes.back().data.primitive.index = (int)id;

  size_t			node = nodes.size() - 1;

  for (std::list<RT::AbstractCsgTree *>::const_iterator it = std::next(children().begin()); it != children().end(); it++)
  {
    primitives[id].data.triangle.index = (int)(*it)->build(nodes, primitives, transformation, material, deph);
    id = (size_t)primitives[id].data.triangle.index;
  }

  return node;
}