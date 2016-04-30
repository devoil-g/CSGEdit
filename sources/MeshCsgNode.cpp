#include <exception>
#include <tuple>
#include <vector>
#include <fstream>

#include "Math.hpp"
#include "MeshCsgNode.hpp"
#include "TriangleCsgLeaf.hpp"

std::map<std::string, void(RT::MeshCsgNode::*)(std::string const &)> const	RT::MeshCsgNode::_extension =
{
  { ".stl", &RT::MeshCsgNode::loadStl }
};

RT::MeshCsgNode::MeshCsgNode()
  : _bound(nullptr)
{}

RT::MeshCsgNode::MeshCsgNode(std::string const & path)
  : _bound(nullptr)
{
  // Load file according to extension
  if (extension(path.substr(path.rfind('.'))))
    (this->*_extension.find(path.substr(path.rfind('.')))->second)(path);
  else
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());
}

RT::MeshCsgNode::~MeshCsgNode()
{}

std::list<RT::Intersection>	RT::MeshCsgNode::renderChildren(RT::Ray const & ray, unsigned int deph) const
{
  // Stop if no intersection with bounding sphere
  if (_bound->render(ray, deph).empty())
    return std::list<RT::Intersection>();

  std::list<RT::Intersection>	intersect;

  // Iterate through triangles to get intersections
  for (RT::AbstractCsgTree const * it : _children)
    intersect.merge(it->render(ray, deph));

  return intersect;
}

void	RT::MeshCsgNode::loadStl(std::string const & path)
{
  std::vector<std::tuple<double, double, double> >	pts;
  std::ifstream	file(path, std::ios::in | std::ios::binary);
  unsigned int	n_tri;
  char		header[80];

  // DO NOT SUPPORT ASCII STL

  if (!file.good())
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  // Render header (80 bytes)
  file.read(header, 80);

  // Read number of triangles (4 bytes)
  file.read((char *)&n_tri, sizeof(unsigned int));
  if (n_tri > 16384)
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  // Get all triangles (50 bytes each)
  for (unsigned int i = 0; i < n_tri; i++)
  {
    float	facet[16];

    file.read((char *)facet, 50);

    push(new TriangleCsgLeaf(std::tuple<double, double, double>(facet[3], facet[4], facet[5]), std::tuple<double, double, double>(facet[6], facet[7], facet[8]), std::tuple<double, double, double>(facet[9], facet[10], facet[11])));

    pts.push_back(std::tuple<double, double, double>(facet[3], facet[4], facet[5]));
    pts.push_back(std::tuple<double, double, double>(facet[6], facet[7], facet[8]));
    pts.push_back(std::tuple<double, double, double>(facet[9], facet[10], facet[11]));
  }

  // Get bounding sphere from points
  _bound = Math::Utils::BoundingSphere(pts);
}

void		RT::MeshCsgNode::push(RT::AbstractCsgTree * node)
{
  if (dynamic_cast<RT::TriangleCsgLeaf *>(node))
    RT::AbstractCsgNode::push(node);
  else
    throw std::exception((std::string(__FILE__) + ": l." + std::to_string(__LINE__)).c_str());

  std::vector<std::tuple<double, double, double>> pts;

  for (RT::AbstractCsgTree const * it : _children)
  {
    std::vector<std::tuple<double, double, double>> pt = dynamic_cast<RT::TriangleCsgLeaf const *>(it)->points();
    
    pts.insert(pts.end(), pt.begin(), pt.end());
  }

  delete _bound;
  _bound = Math::Utils::BoundingSphere(pts);
}

bool		RT::MeshCsgNode::extension(std::string const & ext)
{
  if (_extension.find(ext) != _extension.end())
    return true;
  else
    return false;
}
