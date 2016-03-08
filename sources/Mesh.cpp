#include "Mesh.hpp"
#include "MeshNode.hpp"

RT::Mesh::Mesh()
{}

RT::Mesh::~Mesh()
{
  clear();
}

RT::Mesh &	RT::Mesh::Instance()
{
  static RT::Mesh  singleton;

  // Return unique instance (singleton)
  return singleton;
}

RT::AbstractTree const * const &  RT::Mesh::get(std::string const & file)
{
  // If mesh not in library, load it
  if (_library.find(file) == _library.end())
    _library[file] = std::pair<RT::FileTime, RT::AbstractTree *>(RT::FileTime(file), new RT::MeshNode(file));

  // If mesh has been modified, reload it
  if (_library[file].first.update())
  {
    delete _library[file].second;
    _library[file].second = new RT::MeshNode(file);
  }

  // Return mesh
  return _library[file].second;
}

void	RT::Mesh::clear()
{
  // Delete every mesh loaded in library
  for (std::map<std::string, std::pair<RT::FileTime, RT::AbstractTree *>>::iterator it = _library.begin(); it != _library.end(); it++)
    delete it->second.second;

  // Reset library
  _library.clear();
}