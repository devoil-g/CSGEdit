#ifndef _TRANSFORMATION_NODE_HPP_
#define _TRANSFORMATION_NODE_HPP_

#include <list>

#include "AbstractNode.hpp"
#include "Intersection.hpp"
#include "Matrix.hpp"

namespace RT
{
  class TransformationNode : public RT::AbstractNode
  {
  private:
    Math::Matrix<4, 4>		_transformation;
    
    std::list<RT::Intersection>	renderChildren(Math::Ray const &) const override;  // Render sub-tree

  public:
    TransformationNode(Math::Matrix<4, 4> const &);
    ~TransformationNode();

    std::string			dump() const override;
  };
};

#endif