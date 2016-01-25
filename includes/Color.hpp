#ifndef _COLOR_HPP_
#define _COLOR_HPP_

#include <SFML/Graphics/Color.hpp>
#include <string>

namespace RT
{
  class Color
  {
  public:
    Color();
    Color(double);
    Color(double, double, double);
    ~Color();

    double	r, g, b;				// Color components

    sf::Color	sfml() const;				// Convert RT::Color to sfml
    Color	normalize() const;			// Return a normalize color (component 0-1)

    Color	operator+(Color const &) const;		// Color addition
    Color	operator-(Color const &) const;		// Color substraction
    Color	operator*(Color const &) const;		// Color produce
    Color	operator/(Color const &) const;		// Color division

    Color &	operator+=(Color const &);		// Color addition
    Color &	operator-=(Color const &);		// Color substraction
    Color &	operator*=(Color const &);		// Color produce
    Color &	operator/=(Color const &);		// Color division

    bool	operator==(Color const &) const;	// Color comparison
    bool	operator!=(Color const &) const;	// Color comparison
    
    std::string	dump() const;
  };
};

#endif
