#include "Exception.hpp"
#include "Parser.hpp"
#include "SceneLibrary.hpp"

RT::SceneLibrary::SceneLibrary()
{}

RT::SceneLibrary::~SceneLibrary()
{
  clear();
}

RT::SceneLibrary &	RT::SceneLibrary::Instance()
{
  static RT::SceneLibrary	singleton;

  // Return unique instance (singleton)
  return singleton;
}

RT::SceneLibrary::Time	RT::SceneLibrary::time(std::string const & file) const
{
  // Get file time structure depending of platform
#ifdef WIN32
  FILETIME  creation, lastaccess, lastwrite;
  HANDLE    f = CreateFileA(file.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

  if (f == nullptr || GetFileTime(f, &creation, &lastaccess, &lastwrite) == false || CloseHandle(f) == false)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  return lastwrite;
#else
  struct stat	st;

  if (stat(_file.c_str(), &st) < 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));

  return st.st_mtime;
#endif
}

void			RT::SceneLibrary::load(std::string const & file)
{
  // Update file time
  _library[file].first = time(file);

  // Load file
  _library[file].second.clear();
  _library[file].second = RT::Parser::load(file);
}

RT::Scene *		RT::SceneLibrary::get(std::string const & file)
{
  // Load file if not in cache
  if (_library.find(file) == _library.end())
    load(file);
  
  return &_library[file].second;
}

bool			RT::SceneLibrary::change() const
{
  // Search for a modified file
  for (std::pair<std::string, std::pair<RT::SceneLibrary::Time, RT::Scene>> const & it : _library)
  {
    RT::SceneLibrary::Time	t = time(it.first);

#ifdef _WIN32
    if (it.second.first.dwHighDateTime < t.dwHighDateTime || (it.second.first.dwHighDateTime == t.dwHighDateTime && it.second.first.dwLowDateTime < t.dwLowDateTime))
#else
    if (_library[it.first].first < t)
#endif
      return true;
  }

  return false;
}

void			RT::SceneLibrary::update()
{
  // Reload modified files
  for (std::pair<std::string, std::pair<RT::SceneLibrary::Time, RT::Scene>> const & it : _library)
  {
    RT::SceneLibrary::Time	t = time(it.first);

#ifdef _WIN32
    if (it.second.first.dwHighDateTime < t.dwHighDateTime || (it.second.first.dwHighDateTime == t.dwHighDateTime && it.second.first.dwLowDateTime < t.dwLowDateTime))
#else
    if (_library[it.first].first < t)
#endif
      load(it.first);
  }
}

void			RT::SceneLibrary::clear()
{
  // Delete every scene loaded in library
  for (std::pair<std::string, std::pair<RT::SceneLibrary::Time, RT::Scene>> it : _library)
    it.second.second.clear();

  // Reset library
  _library.clear();
}
