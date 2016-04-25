#ifdef _DEBUG
#include "Exception.hpp"
#endif

#include "Color.hpp"

RT::Color::Color()
  : r(0.f), g(0.f), b(0.f)
{}

RT::Color::Color(double c)
  : r(c), g(c), b(c)
{}

RT::Color::Color(double r, double g, double b)
  : r(r), g(g), b(b)
{}

RT::Color::Color(sf::Color clr)
  : r((double)clr.r / 255.f), g((double)clr.g / 255.f), b((double)clr.b / 255.f)
{}

RT::Color::~Color()
{}

sf::Color	RT::Color::transparent()
{
  return sf::Color(0, 0, 0, 0);
}

sf::Color	RT::Color::sfml() const
{
  RT::Color	n;

  n = normalize();

  // Return sfml color object
  return sf::Color((sf::Uint8)(n.r * 255.f + 0.5f), (sf::Uint8)(n.g * 255.f + 0.5f), (sf::Uint8)(n.b * 255.f + 0.5f));
};

RT::Color	RT::Color::grey() const
{
  return RT::Color(r * 0.2126f + g * 0.7152f + b * 0.0722f);
}

RT::Color	RT::Color::normalize() const
{
  double	r = this->r;
  double	g = this->g;
  double	b = this->b;

  // Check if components positive
  if (r < 0.f)
    r = 0.f;
  if (g < 0.f)
    g = 0.f;
  if (b < 0.f)
    b = 0.f;

  // Check for components saturation
  if (r > 1.f)
    r = 1.f;
  if (g > 1.f)
    g = 1.f;
  if (b > 1.f)
    b = 1.f;

  return RT::Color(r, g, b);
}

RT::Color	RT::Color::operator+(RT::Color const & clr) const
{
  // Components addition
  return RT::Color(this->r + clr.r, this->g + clr.g, this->b + clr.b);
}

RT::Color	RT::Color::operator-(RT::Color const & clr) const
{
  // Components addition
  return RT::Color(this->r - clr.r, this->g - clr.g, this->b - clr.b);
}

RT::Color	RT::Color::operator*(RT::Color const & clr) const
{
  // Components produce
  return RT::Color(this->r * clr.r, this->g * clr.g, this->b * clr.b);
}

RT::Color	RT::Color::operator/(RT::Color const & clr) const
{
#ifdef _DEBUG
  // Check for division by 0
  if (clr.r == 0 || clr.g == 0 || clr.b == 0)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

  // Components division
  return RT::Color(this->r / clr.r, this->g / clr.g, this->b / clr.b);
}

RT::Color &	RT::Color::operator+=(RT::Color const & clr)
{
  // Components addition
  this->r += clr.r;
  this->g += clr.g;
  this->b += clr.b;
  return *this;
}

RT::Color &	RT::Color::operator-=(RT::Color const & clr)
{
  // Components substraction
  this->r -= clr.r;
  this->g -= clr.g;
  this->b -= clr.b;
  return *this;
}

RT::Color &	RT::Color::operator*=(RT::Color const & clr)
{
  // Components produce
  this->r *= clr.r;
  this->g *= clr.g;
  this->b *= clr.b;
  return *this;
}

RT::Color &	RT::Color::operator/=(RT::Color const & clr)
{
#ifdef _DEBUG
  // Check for division by 0
  if (clr.r == 0.f || clr.g == 0.f || clr.b == 0.f)
    throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

  // Components division
  this->r /= clr.r;
  this->g /= clr.g;
  this->b /= clr.b;
  return *this;
}

bool  RT::Color::operator==(RT::Color const & clr) const
{
  return r == clr.r && g == clr.g && b == clr.b;
}

bool  RT::Color::operator!=(RT::Color const & clr) const
{
  return r != clr.r || g != clr.g || b != clr.b;
}
