#include "Exception.hpp"
#include "FileTime.hpp"

RT::FileTime::FileTime()
  : _time(), _file()
{}

RT::FileTime::FileTime(std::string const & file)
  : _time(), _file(file)
{
  update();
}

RT::FileTime::~FileTime()
{}

bool	RT::FileTime::update()
{
  if (_file == "")
    return false;

#ifdef WIN32
  FILETIME  creation, lastaccess, lastwrite;
  HANDLE    f = CreateFileA(_file.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

  if (f == NULL)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  GetFileTime(f, &creation, &lastaccess, &lastwrite);
  CloseHandle(f);

  if (lastwrite.dwHighDateTime > _time.dwHighDateTime || (lastwrite.dwHighDateTime == _time.dwHighDateTime && lastwrite.dwLowDateTime > _time.dwLowDateTime))
  {
    _time = lastwrite;
    return true;
  }
#else
  struct stat	st;

  if (stat(_file.c_str(), &st) < 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  if (st.st_mtime > _time)
  {
    _time = st.st_mtime;
    return true;
  }
#endif

  return false;
}

std::string const & RT::FileTime::file() const
{
  return _file;
}