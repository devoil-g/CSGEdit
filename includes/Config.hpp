#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#include <string>

#include "Color.hpp"

namespace RT
{
  namespace Config
  {
    void		initialize(int, char **);

    extern std::string	ExecutablePath;

    std::string	const	WindowTitle("WorldEdit");
    std::string	const	WindowIcon("C:/project/CSG-Raytracer/assets/icons/icon16.png");
    unsigned int const	WindowWidth(640);
    unsigned int const	WindowHeight(360);
    unsigned int const	ThreadSize(8);
    unsigned int const	ThreadNumber(3);
    double const	ThreadSleep(0.5f);

    unsigned int const	AntiAliasing(2);
    unsigned int const	MaxRecursivite(3);

    namespace Light
    {
      RT::Color const	Ambient(0.4f);
      RT::Color const	Diffuse(0.6f);
      RT::Color const	Specular(0.2f);
      unsigned const	DefaultQuality(3);
      double const	DefaultAmbientRadius(32.f);
    };

    namespace Anaglyph3D
    {
      double const		Offset(0.f);
      double const		Focal(700.f);
      RT::Color const		MaskLeft(1.f, 0.f, 0.f);
      RT::Color const		MaskRight(0.f, 1.f, 1.f);
    };

    namespace DephOfField
    {
      unsigned int const	Quality(1);
      double const		Aperture(16.f);
      double const		Focal(RT::Config::Anaglyph3D::Focal);
    };
  };
};

#endif
