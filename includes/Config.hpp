#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <string>

#include "Color.hpp"

namespace RT
{
  namespace Config
  {
    void		initialize(int, char **);

    extern std::string	ExecutablePath;
    extern unsigned int	ThreadNumber;
  };
};

#endif
