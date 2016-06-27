#ifndef _ABSTRACT_LIGHT_LEAF_HPP_
#define _ABSTRACT_LIGHT_LEAF_HPP_

#include "AbstractLightTree.hpp"

namespace RT
{
  class AbstractLightLeaf : public RT::AbstractLightTree
  {
  public:
    AbstractLightLeaf();
    virtual ~AbstractLightLeaf();
  };
};

#endif
