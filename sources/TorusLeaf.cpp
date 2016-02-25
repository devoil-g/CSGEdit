#include <sstream>

#include "Math.hpp"
#include "TorusLeaf.hpp"

RT::TorusLeaf::TorusLeaf(double r, double h)
  : _r(r), _h(h)
{}

RT::TorusLeaf::~TorusLeaf()
{}

std::vector<double>	RT::TorusLeaf::intersection(RT::Ray const & ray) const
{
  // Not so clever equations
  std::vector<double>	result = Math::Utils::solve(
    std::pow(ray.d().x() * ray.d().x() + ray.d().y() * ray.d().y() + ray.d().z() * ray.d().z(), 2.f),
    4.f * (ray.d().x() * ray.p().x() + ray.d().y() * ray.p().y() + ray.d().z() * ray.p().z()) * (ray.d().x() * ray.d().x() + ray.d().y() * ray.d().y() + ray.d().z() * ray.d().z()),
    2.f * (ray.d().x() * ray.d().x() + ray.d().y() * ray.d().y() + ray.d().z() * ray.d().z()) * (ray.p().x() * ray.p().x() + ray.p().y() * ray.p().y() + ray.p().z() * ray.p().z() + _r * _r - _h * _h) + 4.f * std::pow(ray.d().x() * ray.p().x() + ray.d().y() * ray.p().y() + ray.d().z() * ray.p().z(), 2.f) - 4.f * (ray.d().x() * ray.d().x() + ray.d().y() * ray.d().y()) * _r * _r,
    4.f * (ray.p().x() * ray.p().x() + ray.p().y() * ray.p().y() + ray.p().z() * ray.p().z() + _r * _r - _h * _h) * (ray.d().x() * ray.p().x() + ray.d().y() * ray.p().y() + ray.d().z() * ray.p().z()) - 8.f * (ray.d().x() * ray.p().x() + ray.d().y() * ray.p().y()) * _r * _r,
    std::pow(ray.p().x() * ray.p().x() + ray.p().y() * ray.p().y() + ray.p().z() * ray.p().z() + _r * _r - _h * _h, 2.f) - 4.f * (ray.p().x() * ray.p().x() + ray.p().y() * ray.p().y()) * _r * _r
    );

  // If torus overlap on himself, delete inside intersections
  if (_h > _r && result.size() == 4)
  {
    std::list<double>	list;
    
    for (unsigned int i = 0; i < 4; i++)
      list.push_back(result[i]);
    list.sort();

    if (std::abs(ray.p().z() + ray.d().z() * (*std::next(list.begin()))) < std::sqrt(1 - ((_r * _r) / (_h * _h))) * _h)
      return { list.front(), list.back() };
  }

  return result;
}

Math::Vector<4>		RT::TorusLeaf::normal(Math::Vector<4> const & pt) const
{
  Math::Vector<4>	n;
  
  // Point perfectly centered
  if (pt.x() == 0.f && pt.y() == 0.f)
    n.z() = 1.f;
  else
  {
    // Simple method to get n
    double	c = _r / std::sqrt(pt.x() * pt.x() + pt.y() * pt.y());

    if (std::abs(std::sqrt(std::pow(pt.x() - (pt.x() * c), 2.f) + std::pow(pt.y() - (pt.y() * c), 2.f) + std::pow(pt.z(), 2.f)) - _h) < Math::Shift)
    {
      n.x() = pt.x() - pt.x() * c;
      n.y() = pt.y() - pt.y() * c;
      n.z() = pt.z();
    }
    else
    {
      n.x() = pt.x() + pt.x() * c;
      n.y() = pt.y() + pt.y() * c;
      n.z() = pt.z();
    }
  }

  return n;
}

std::string	RT::TorusLeaf::dump() const
{
  std::stringstream stream;

  stream << "torus(" << _r << ", " << _h << ");";

  return stream.str();
}