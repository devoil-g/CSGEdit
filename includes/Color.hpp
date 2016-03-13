#ifndef _COLOR_HPP_
#define _COLOR_HPP_

#include <SFML/Graphics/Color.hpp>

namespace RT
{
  class Color
  {
  public:
    Color();
    Color(double);
    Color(double, double, double);
    Color(sf::Color);
    ~Color();

    double	r, g, b;				// Color components

    sf::Color	sfml() const;				// Convert RT::Color to sfml
    RT::Color	normalize() const;			// Return a normalize color (component 0-1)

    RT::Color	operator+(Color const &) const;		// Color addition
    RT::Color	operator-(Color const &) const;		// Color substraction
    RT::Color	operator*(Color const &) const;		// Color produce
    RT::Color	operator/(Color const &) const;		// Color division

    RT::Color &	operator+=(Color const &);		// Color addition
    RT::Color &	operator-=(Color const &);		// Color substraction
    RT::Color &	operator*=(Color const &);		// Color produce
    RT::Color &	operator/=(Color const &);		// Color division

    bool	operator==(Color const &) const;	// Color comparison
    bool	operator!=(Color const &) const;	// Color comparison
  };
};

#endif
