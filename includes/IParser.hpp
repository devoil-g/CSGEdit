#ifndef _I_PARSER_HPP_
#define _I_PARSER_HPP_

#include <string>

#include "AbstractLeaf.hpp"
#include "AbstractNode.hpp"
#include "AbstractTree.hpp"
#include "ICreator.hpp"
#include "Math.hpp"

namespace RT
{
  class IParser
  {
    const RT::ICreator const *	_creator;

  public:
    Parser(ICreator const * creator) : _creator(creator) {};

    virtual RT::AbstractTree *	load(std::string const &) const = 0;
  };
};

#endif
