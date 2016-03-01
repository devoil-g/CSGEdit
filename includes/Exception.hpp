#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <exception>
#include <string>

namespace RT
{
  class Exception : public std::exception
  {
  private:
    std::string const	_msg;	// Exception description

  public:
    Exception(std::string const &);
    ~Exception();

    const char *	what() const throw();	// Get execption message
  };
};

#endif
