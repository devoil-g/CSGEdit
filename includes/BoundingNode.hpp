#ifndef _BOUNDING_NODE_HPP_
#define _BOUNDING_NODE_HPP_

#include <list>

#include "AbstractNode.hpp"
#include "AbstractTree.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"

namespace RT
{
  class BoundingNode : public AbstractNode
  {
  private:
    AbstractTree const *  _bound;

    std::list<RT::Intersection>	renderTree(Math::Ray const &) const override;

  public:
    BoundingNode(AbstractTree const *);
    BoundingNode(Math::Matrix<4, 4> const &, AbstractTree const *);
    ~BoundingNode();

    std::string		dump() const override;
  };
};

#endif