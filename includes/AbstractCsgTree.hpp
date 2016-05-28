#ifndef _ABSTRACT_CSG_TREE_HPP_
#define _ABSTRACT_CSG_TREE_HPP_

#include <list>

#include "Intersection.hpp"
#include "Material.hpp"
#include "OpenCLStructures.hpp"
#include "Ray.hpp"

namespace RT
{
  class AbstractCsgTree
  {
  public:
    AbstractCsgTree();
    virtual ~AbstractCsgTree();

    virtual std::list<RT::Intersection>	render(RT::Ray const &, unsigned int = 0) const = 0;														// Render a list of intersection from the sub-tree according to ray
    virtual size_t			build(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &, Math::Matrix<4, 4> const &, RT::Material const &, unsigned int = 0) const = 0;	// Build OpenCL data structure
  };
};

#endif
