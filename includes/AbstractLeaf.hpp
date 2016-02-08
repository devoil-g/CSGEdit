#ifndef _ABSTRACT_LEAF_HPP_
#define _ABSTRACT_LEAF_HPP_

#include <list>
#include <vector>

#include "AbstractTree.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"

namespace RT
{
  class AbstractLeaf : public RT::AbstractTree
  {
  private:
    virtual std::vector<double>	intersection(Math::Ray const &) const = 0;	// Render intersection distance form primitivee. You should override this.
    virtual Math::Ray		normal(Math::Ray const &) const = 0;		// Calculate normal vector from intersection point. You should override this.

  public:
    AbstractLeaf();
    virtual ~AbstractLeaf();

    std::list<RT::Intersection>	render(Math::Ray const &) const override;	// Render primitive according to ray
  };
};

#endif
