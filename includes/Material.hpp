#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include <map>
#include <string>

#include "Color.hpp"

namespace RT
{
  class Material
  {
  private:
    static std::map<std::string, RT::Material>	_material;	// Material mapped by name

  public:
    RT::Color	color;		// Color of material
    RT::Color	ambient;	// Ambient component multiplier
    RT::Color	diffuse;	// Diffuse component multiplier
    RT::Color	specular;	// Specular component multiplier
    double	shine;		// Shine coeffient of material
    double	reflection;	// Reflection coefficient (0-1)
    double	refraction;	// Index of refraction (+0-inf)
    double	transparency;	// Transparency coefficient (0-1)

    Material();
    ~Material();

    static RT::Material	const &	getMaterial(std::string const &);			// Get material from his name
    static void			setMaterial(std::string const &, RT::Material const &);	// Set material properties
    static void			initialize();						// Initialize all materials

    RT::Material		operator*(RT::Material const &) const;			// Multiply materials
    RT::Material &		operator*=(RT::Material const &);			// Multiply materials
  };
};

#endif
