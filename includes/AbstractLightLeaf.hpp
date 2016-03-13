#ifndef _ABSTRACT_LIGHT_LEAF_HPP_
#define _ABSTRACT_LIGHT_LEAF_HPP_

#include "AbstractLightTree.hpp"

namespace RT
{
  class AbstractLightLeaf : public AbstractLightTree
  {
  public:
    AbstractLightLeaf();
    virtual ~AbstractLightLeaf();
  };
};

#endif
