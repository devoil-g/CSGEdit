#include "Exception.hpp"

RT::Exception::Exception(std::string const & msg)
  : _msg(msg)
{};

RT::Exception::~Exception()
{}

const char *	RT::Exception::what() const throw()
{
  return _msg.c_str();
}
