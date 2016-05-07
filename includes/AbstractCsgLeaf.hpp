#ifndef _ABSTRACT_CSG_LEAF_HPP_
#define _ABSTRACT_CSG_LEAF_HPP_

#include <list>
#include <vector>

#include "AbstractCsgTree.hpp"
#include "Intersection.hpp"
#include "Ray.hpp"
#include "Vector.hpp"

namespace RT
{
  class AbstractCsgLeaf : public RT::AbstractCsgTree
  {
  private:
    virtual std::vector<double>	intersection(RT::Ray const &) const = 0;	// Render intersection distance of primitive. You should override this.
    virtual Math::Vector<4>	normal(Math::Vector<4> const &) const = 0;	// Calculate normal vector from intersection point. You should override this.

  public:
    AbstractCsgLeaf();
    virtual ~AbstractCsgLeaf();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render primitive according to ray
  };
};

#endif
