#ifndef _I_CREATOR_HPP_
#define _I_CREATOR_HPP_

#include <tuple>
#include <vector>

#include "AbstractLeaf.hpp"
#include "AbstractNode.hpp"
#include "AbstractTree.hpp"
#include "Math.hpp"

namespace RT
{
  class ICreator
  {
  public:
    AbstractTree *	createBox(Math::Matrix const &, double, double, double, bool) const = 0;
    AbstractTree *	createCone(Math::Matrix const &, double, double, double, bool) const = 0;
    AbstractTree *	createMesh(Math::Matrix const &, std::vector<std::tuple<double, double, double> > const &, std::vector<std::tuple<unsigned int, unsigned int, unsigned int> > const &) const = 0;
    AbstractTree *	createSphere(Math::Matrix const &, double) const = 0;

    AbstractTree *	createUnion(std:vector<AbstractTree *> const &) const = 0;
    AbstractTree *	createDifference(std:vector<AbstractTree *> const &) const = 0;
    AbstractTree *	createIntersection(std:vector<AbstractTree *> const &) const = 0;
  };
};

#endif
