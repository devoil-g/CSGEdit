#ifndef _CSG_NODE_HPP_
#define _CSG_NODE_HPP_

#include <list>

#include "AbstractNode.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"

namespace RT
{
  class CsgNode : public RT::AbstractNode
  {
  public:
    enum Type
    {
      Union,
      Intersect,
      Difference
    };

  private:
    Type const	_type;	// Store CSG type

    std::list<RT::Intersection>	renderUnion(std::list<std::list<RT::Intersection> > &) const;	      // Render union from sub-tree
    std::list<RT::Intersection>	renderIntersection(std::list<std::list<RT::Intersection> > &) const;  // Render intersection from sub-tree
    std::list<RT::Intersection>	renderDifference(std::list<std::list<RT::Intersection> > &) const;    // Render difference from sub-tree

    std::list<RT::Intersection>	renderTree(Math::Ray const &) const override;  // Render sub-tree

  public:
    CsgNode(Type);
    CsgNode(Math::Matrix<4, 4> const &, Type);
    ~CsgNode();
  };

  RT::AbstractTree const *  createUnion(RT::AbstractTree const *, RT::AbstractTree const *);
  RT::AbstractTree const *  createDifference(RT::AbstractTree const *, RT::AbstractTree const *);
  RT::AbstractTree const *  createIntersection(RT::AbstractTree const *, RT::AbstractTree const *);
};

#endif
