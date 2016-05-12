#ifndef _DEPH_LIGHT_NODE_HPP_
#define _DEPH_LIGHT_NODE_HPP_

#include "UnionLightNode.hpp"

namespace RT
{
  class DephLightNode : public RT::UnionLightNode
  {
  private:
    unsigned int const	_deph;	// Maximum deph before interrupting rendering

  public:
    DephLightNode(unsigned int);
    ~DephLightNode();

    RT::Color		preview(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Return preview lighting for given intersection
    RT::Color		render(Math::Matrix<4, 4> const &, RT::Scene const *, RT::Ray const &, RT::Intersection const &, unsigned int, unsigned int) const override;	// Return rendered lighting for given intersection
  };
};

#endif
