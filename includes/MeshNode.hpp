#ifndef _MESH_NODE_HPP_
#define _MESH_NODE_HPP_

#include <list>
#include <string>

#include "AbstractNode.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"

namespace RT
{
  class MeshNode : public RT::AbstractNode
  {
  private:
    RT::AbstractTree const *  _bound;	// Bounding sphere containing mesh faces (for acceleration purpose)

    std::list<RT::Intersection>	renderChildren(Math::Ray const &) const override;  // Render sub-tree
    void  loadStl(std::string const &); // Load a .stl file

  public:
    MeshNode(std::string const &);
    ~MeshNode();

    std::string			dump() const override;
  };
};

#endif
