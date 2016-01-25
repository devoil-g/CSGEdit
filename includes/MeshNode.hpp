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

    void  loadStl(std::string const &); // Load a .stl file

    std::list<RT::Intersection>	renderTree(Math::Ray const &) const;  // Render intersection with mesh faces

  public:
    MeshNode(std::string const &);
    MeshNode(Math::Matrix<4, 4> const &, std::string const &);
    ~MeshNode();

    std::string		dump() const override;
  };
};

#endif
