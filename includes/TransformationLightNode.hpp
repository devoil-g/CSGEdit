#ifndef _TRANSFORMATION_LIGHT_NODE_HPP_
#define _TRANSFORMATION_LIGHT_NODE_HPP_

#include "UnionLightNode.hpp"
#include "Matrix.hpp"

namespace RT
{
  class TransformationLightNode : public RT::UnionLightNode
  {
  private:
    Math::Matrix<4, 4>		_transformation;	// Transformation applied to sub-tree

  public:
    TransformationLightNode(Math::Matrix<4, 4> const &);
    ~TransformationLightNode();

    RT::Color			preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Return preview lighting for given intersection
    RT::Color			render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Return rendered lighting for given intersection

    // Getter/setter of transformation matrix
    Math::Matrix<4, 4> &	transformation() { return _transformation; };
    Math::Matrix<4, 4> const &	transformation() const { return _transformation; };
  };
};

#endif
