#ifndef _MESH_CSG_NODE_HPP_
#define _MESH_CSG_NODE_HPP_

#include <list>
#include <map>
#include <string>

#include "AbstractCsgNode.hpp"
#include "AbstractCsgTree.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class MeshCsgNode : public RT::AbstractCsgNode
  {
  private:  
    static std::map<std::string, void(RT::MeshCsgNode::*)(std::string const &)> const	_extension;

    RT::AbstractCsgTree const *	_bound;	// Bounding sphere containing mesh faces (for acceleration purpose)

    std::list<RT::Intersection>	renderChildren(RT::Ray const &, unsigned int) const override;	// Render sub-tree
    
    void	loadStl(std::string const &);			// Load a .stl file

  public:
    MeshCsgNode();
    MeshCsgNode(std::string const &);
    ~MeshCsgNode();

    void			push(RT::AbstractCsgTree *) override;	// Push a triangle in mesh

    static bool			extension(std::string const &);	// Return true if extension supported
  };
};

#endif
