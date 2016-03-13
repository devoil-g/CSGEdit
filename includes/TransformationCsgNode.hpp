#ifndef _TRANSFORMATION_CSG_NODE_HPP_
#define _TRANSFORMATION_CSG_NODE_HPP_

#include "AbstractCsgNode.hpp"
#include "Matrix.hpp"

namespace RT
{
  class TransformationCsgNode : public RT::AbstractCsgNode
  {
  private:
    Math::Matrix<4, 4>		_transformation;	// Transformation applied to sub-tree
    
    std::list<RT::Intersection>	renderChildren(RT::Ray const &, unsigned int) const override;	// Render sub-tree

  public:
    TransformationCsgNode(Math::Matrix<4, 4> const &);
    ~TransformationCsgNode();

    Math::Matrix<4, 4>		transformation() const;	// Return transformation matrix of node
  };
};

#endif
