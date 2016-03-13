#ifndef _ABSTRACT_CSG_TREE_HPP_
#define _ABSTRACT_CSG_TREE_HPP_

#include <list>
#include <string>

#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  namespace Config
  {
    namespace Csg
    {
      unsigned int const	MaxDeph(8);	// Maximum of sub-file CSG tree rendered
    };
  };

  class AbstractCsgTree
  {
  public:
    AbstractCsgTree();
    virtual ~AbstractCsgTree();

    virtual std::list<RT::Intersection>	render(RT::Ray const &, unsigned int = 0) const = 0;	// Render a list of intersection from the sub-tree according to ray
  };
};

#endif
