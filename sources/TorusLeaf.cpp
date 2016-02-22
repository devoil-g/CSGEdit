#include <sstream>

#include "Math.hpp"
#include "TorusLeaf.hpp"

RT::TorusLeaf::TorusLeaf(double r, double h)
  : _r(r), _h(h)
{}

RT::TorusLeaf::~TorusLeaf()
{}

std::vector<double>	RT::TorusLeaf::intersection(Math::Ray const & ray) const
{
  std::vector<double>	result;
  double		a, b, c, d, e;

  // Not so clever equations
  a = std::pow(ray.dx() * ray.dx() + ray.dy() * ray.dy() + ray.dz() * ray.dz(), 2.f);
  b = 4.f * (ray.dx() * ray.px() + ray.dy() * ray.py() + ray.dz() * ray.pz()) * (ray.dx() * ray.dx() + ray.dy() * ray.dy() + ray.dz() * ray.dz());
  c = 2.f * (ray.dx() * ray.dx() + ray.dy() * ray.dy() + ray.dz() * ray.dz()) * (ray.px() * ray.px() + ray.py() * ray.py() + ray.pz() * ray.pz() + _r * _r - _h * _h) + 4.f * std::pow(ray.dx() * ray.px() + ray.dy() * ray.py() + ray.dz() * ray.pz(), 2.f) - 4.f * (ray.dx() * ray.dx() + ray.dy() * ray.dy()) * _r * _r;
  d = 4.f * (ray.px() * ray.px() + ray.py() * ray.py() + ray.pz() * ray.pz() + _r * _r - _h * _h) * (ray.dx() * ray.px() + ray.dy() * ray.py() + ray.dz() * ray.pz()) - 8.f * (ray.dx() * ray.px() + ray.dy() * ray.py()) * _r * _r;
  e = std::pow(ray.px() * ray.px() + ray.py() * ray.py() + ray.pz() * ray.pz() + _r * _r - _h * _h, 2.f) - 4.f * (ray.px() * ray.px() + ray.py() * ray.py()) * _r * _r;
  result = Math::Utils::solve(a, b, c, d, e);

  // If torus overlap on himself, delete inside intersections
  if (_h > _r && result.size() == 4)
  {
    std::list<double>			list;
    std::list<double>::const_iterator	it;
    double				h;

    for (unsigned int i = 0; i < 4; i++)
      list.push_back(result[i]);
    list.sort();

    h = std::sqrt(1 - ((_r * _r) / (_h * _h))) * _h;

    it = list.begin();
    it++;

    if (std::abs(ray.pz() + ray.dz() * (*it)) < h)
    {
      std::vector<double>	tmp;

      tmp.push_back(list.front());
      tmp.push_back(list.back());
      return tmp;
    }

    return result;
  }
  else
    return result;
}

Math::Ray	RT::TorusLeaf::normal(Math::Ray const & ray) const
{
  Math::Ray	normal;
  double	c;

  // Point perfectly centered
  if (ray.px() == 0 && ray.py() == 0)
  {
    normal.dx() = 0;
    normal.dy() = 0;
    normal.dz() = 1;
  }
  else
  {
    // Simple method to get normal
    c = _r / std::sqrt(ray.px() * ray.px() + ray.py() * ray.py());

    if (std::abs(std::sqrt(std::pow(ray.px() - (ray.px() * c), 2.f) + std::pow(ray.py() - (ray.py() * c), 2.f) + std::pow(ray.pz(), 2.f)) - _h) < Math::Shift)
    {
      normal.dx() = ray.px() - ray.px() * c;
      normal.dy() = ray.py() - ray.py() * c;
      normal.dz() = ray.pz();
    }
    else
    {
      normal.dx() = ray.px() + ray.px() * c;
      normal.dy() = ray.py() + ray.py() * c;
      normal.dz() = ray.pz();
    }
  }

  return normal;
}

std::string	RT::TorusLeaf::dump() const
{
  std::stringstream stream;

  stream << "torus(" << _r << ", " << _h << ");";

  return stream.str();
}