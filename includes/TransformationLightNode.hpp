#ifndef _TRANSFORMATION_LIGHT_NODE_HPP_
#define _TRANSFORMATION_LIGHT_NODE_HPP_

#include "AbstractLightNode.hpp"
#include "Matrix.hpp"

namespace RT
{
  class TransformationLightNode : public RT::AbstractLightNode
  {
  private:
    Math::Matrix<4, 4>	_transformation;	// Transformation applied to sub-tree

  public:
    TransformationLightNode(Math::Matrix<4, 4> const &);
    ~TransformationLightNode();

    RT::Color		preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int) const override;	// Return preview lighting for given intersection
    RT::Color		render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int) const override;	// Return rendered lighting for given intersection
  };
};

#endif
