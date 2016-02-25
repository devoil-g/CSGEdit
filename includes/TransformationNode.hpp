#ifndef _TRANSFORMATION_NODE_HPP_
#define _TRANSFORMATION_NODE_HPP_

#include <list>
#include <string>

#include "AbstractNode.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"

namespace RT
{
  class TransformationNode : public RT::AbstractNode
  {
  private:
    Math::Matrix<4, 4>		_transformation;					// Transformation applied to sub-tree
    
    std::list<RT::Intersection>	renderChildren(RT::Ray const &) const override;	// Render sub-tree

  public:
    TransformationNode(Math::Matrix<4, 4> const &);
    ~TransformationNode();

    std::string			dump() const override;					// Dump CSG tree
    Math::Matrix<4, 4>		transformation() const;					// Return transformation matrix of node
  };
};

#endif