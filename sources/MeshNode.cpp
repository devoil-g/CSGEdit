#include <map>
#include <tuple>
#include <vector>
#include <fstream>

#include "MeshNode.hpp"
#include "Exception.hpp"
#include "TriangleLeaf.hpp"

#include <iostream>

RT::MeshNode::MeshNode(std::string const & path)
  : _bound(nullptr)
{
  std::string	ext = path.substr(path.rfind('.'));

  // Load file according to extension
  if (!ext.compare(".stl"))
    loadStl(path);
  else
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
}

RT::MeshNode::~MeshNode()
{}

std::list<RT::Intersection>	RT::MeshNode::renderChildren(Math::Ray const & ray) const
{
  std::list<RT::Intersection>		  intersect;

  // Stop if no intersection with bounding sphere
  if (_bound && _bound->render(ray).empty())
    return std::list<RT::Intersection>();

  // Iterate through triangles to get intersections
  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    std::list<RT::Intersection> node = (*it)->render(ray);

//    for (std::list<RT::Intersection>::iterator it_node = node.begin(); it_node != node.end(); it_node++)
//      it_node->node = *it;

    intersect.merge(node);
  }

  return intersect;
}

void	RT::MeshNode::loadStl(std::string const & path)
{
  std::vector<std::tuple<double, double, double> >	pts;
  std::ifstream	file(path, std::ios::in | std::ios::binary);
  unsigned int	n_tri;
  char		header[80];

  // DO NOT SUPPORT ASCII STL

  if (!file.good())
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  // Render header (80 bytes)
  file.read(header, 80);

  // Read number of triangles (4 bytes)
  file.read((char *)&n_tri, sizeof(unsigned int));
  if (n_tri > 16384)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  // Get all triangles (50 bytes each)
  for (unsigned int i = 0; i < n_tri; i++)
  {
    float	facet[16];

    file.read((char *)facet, 50);

    push(new TriangleLeaf(std::tuple<double, double, double>(facet[3], facet[4], facet[5]), std::tuple<double, double, double>(facet[6], facet[7], facet[8]), std::tuple<double, double, double>(facet[9], facet[10], facet[11])));

    pts.push_back(std::tuple<double, double, double>(facet[3], facet[4], facet[5]));
    pts.push_back(std::tuple<double, double, double>(facet[6], facet[7], facet[8]));
    pts.push_back(std::tuple<double, double, double>(facet[9], facet[10], facet[11]));
  }

  // Get bounding sphere from points
  _bound = Math::Utils::BoundingSphere(pts);
}

std::string	RT::MeshNode::dump() const
{
  std::stringstream stream;

  stream << "mesh(){";

  for (std::list<RT::AbstractTree const *>::const_iterator it = _children.begin(); it != _children.end(); it++)
  {
    if (it != _children.begin())
      stream << ", ";
    stream << (*it)->dump();
  }

  stream << "}";

  return stream.str();
}