#ifndef _MATH_HPP_
#define _MATH_HPP_

#define _USE_MATH_DEFINES

#include <vector>
#include <tuple>
#include <ctime>
#include <cmath>
#include <ctgmath>
#include <cstdlib>
#include <memory>

#ifndef M_PI
# define M_PI	3.14159265358979323846
#endif

namespace RT
{
  class AbstractTree;
};

namespace Math
{
  void			initialize();		// Initialize math module

  double const		Shift(0.000001f);	// Constant used to move points to avoid undesired intersection
  double const		Pi(M_PI);		// Use this instead of M_PI
  unsigned int const	RandomTableSize(4096);	// Size of random table
  
  namespace Utils
  {
    double		RadToDeg(double); // Convert radian to degree
    double		DegToRad(double); // Convert degree to radian

    std::vector<double>	solve(double, double, double);			// Solve a 2nd degree equation
    std::vector<double>	solve(double, double, double, double, double);	// Solve a 4th degree equation

    RT::AbstractTree const *	BoundingSphere(std::vector<std::tuple<double, double, double> > const &); // Generate a bounding sphere from points
  };

  class Random
  {
  private:
    static std::vector<double>	_table;	  // Table containing random number between 0 and 1
    static unsigned int		_cursor;  // Current position in table

  public:
    static void		initialize();	  // Initialize random table
    static double	rand(double = 1); // Return a random number between 0 and parameter
  };
};

#endif
