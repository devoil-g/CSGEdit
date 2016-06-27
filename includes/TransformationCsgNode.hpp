#ifndef _TRANSFORMATION_CSG_NODE_HPP_
#define _TRANSFORMATION_CSG_NODE_HPP_

#include "UnionCsgNode.hpp"
#include "Matrix.hpp"

namespace RT
{
  class TransformationCsgNode : public RT::UnionCsgNode
  {
  private:
    Math::Matrix<4, 4>		_transformation;	// Transformation applied to sub-tree

  public:
    TransformationCsgNode(Math::Matrix<4, 4> const &);
    ~TransformationCsgNode();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render sub-tree

    // Getter/setter of transformation matrix
    Math::Matrix<4, 4> &	transformation() { return _transformation; };
    Math::Matrix<4, 4> const &	transformation() const { return _transformation; };
  };
};

#endif
