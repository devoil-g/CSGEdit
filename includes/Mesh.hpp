#ifndef _MESH_HPP_
#define _MESH_HPP_

#include <map>
#include <string>
#include <utility>

#include "AbstractTree.hpp"
#include "FileTime.hpp"

namespace RT
{
  class Mesh
  {
  private:
    std::map<std::string, std::pair<RT::FileTime, RT::AbstractTree *>>	_library;	// Cache of mesh loaded
    
    RT::AbstractTree *	load(std::string const &) const;	// Load a mesh

    Mesh();
    ~Mesh();

  public:
    static RT::Mesh &			Instance();			// Get instance (singleton)

    RT::AbstractTree const * const &	get(std::string const &);	// Get/update a mesh
    void				clear();			// Clear cache
  };
};

#endif
