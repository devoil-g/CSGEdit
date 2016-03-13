#include <ctime>
#include <limits>

#ifdef _DEBUG
#include "Exception.hpp"
#endif

#include "Math.hpp"
#include "Matrix.hpp"
#include "SphereCsgLeaf.hpp"
#include "TransformationCsgNode.hpp"

void	Math::initialize()
{
  // Initialize random module
  Math::Random::initialize();
}

std::vector<double>	Math::Utils::solve(double a, double b, double c)
{ 
#ifdef _DEBUG
  // Check for invalid parameter
  if (a == 0)
    return std::vector<double>();
#endif

  // Solve equation
  double  d = b * b - 4.f * a * c;
  if (d >= 0)
    return { (-b - std::sqrt(d)) / (2.f * a), (-b + std::sqrt(d)) / (2.f * a) };
  else
    return {};
}

std::vector<double>	Math::Utils::solve(double a, double b, double c, double d, double e)
{
#ifdef _DEBUG
  // Check for invalid parameter
  if (a == 0)
    return std::vector<double>();
#endif

  // Solve equation
  double  delta = b / (2.f * a);

  double  f = c / a - 3.f * delta * delta / 2.f;
  double  g = d / a + delta * delta * delta - c * delta / a;
  double  h = e / a - 3.f * delta * delta * delta * delta / 16.f + c * delta * delta / (4.f * a) - d * delta / (2.f * a);
  double  i = -2.f * f * f * f / 27.f - g * g + 8.f * f * h / 3.f;
  double  j = -(f * f + 12.f * h) / 3.f;

  delta = j * j * j / 27.f + i * i / 4.f;
  if (delta > 0)
    delta = std::cbrt(-i / 2.f + std::sqrt(delta)) - (j / 3.f) / std::cbrt(-i / 2.f + std::sqrt(delta));
  else if (delta == 0)
    delta = 3.f * i / j;
  else
    delta = 2.f * std::sqrt(-j / 3.f) * std::cos(std::acos(-i / 2.f / std::pow((-j / 3.f), (3.f / 2.f))) / 3.f);

  double  k = f / 3.f + delta;
  double  l = std::sqrt(k - f);
  double  m = std::sqrt(std::pow(k / 2.f, 2.f) - h);
  double  n = -b / (4.f * a);

  delta = l * l - 2.f * k + 4.f * m;
  if (delta < 0.f)
    return {};

  double  x1 = l / 2.f;
  double  x2 = std::sqrt(delta) / 2.f;
  if (g > 0)
    x1 *= -1.f;

  delta = l * l - 2.f * k - 4.f * m;
  if (delta < 0.f)
    return { x1 - x2 + n, x1 + x2 + n };

  double  y1 = -l / 2.f;
  double  y2 = std::sqrt(delta) / 2.f;
  if (g > 0)
    y1 *= -1.f;

  return { x1 - x2 + n, x1 + x2 + n, y1 - y2 + n, y1 + y2 + n };
}

RT::AbstractCsgTree const *  Math::Utils::BoundingSphere(std::vector<std::tuple<double, double, double> > const & pts)
{
  std::tuple<double, double, double>	xmin, xmax, ymin, ymax, zmin, zmax;
  std::tuple<double, double, double>	dia1, dia2, center;
  double				xSpan, ySpan, zSpan, maxSpan;
  double				sqRad, radius;

  // Calculate a bounding sphere 
  std::get<0>(xmin) = std::numeric_limits<double>::max();
  std::get<1>(ymin) = std::numeric_limits<double>::max();
  std::get<2>(zmin) = std::numeric_limits<double>::max();
  std::get<0>(xmax) = std::numeric_limits<double>::min();
  std::get<1>(ymax) = std::numeric_limits<double>::min();
  std::get<2>(zmax) = std::numeric_limits<double>::min();

  for (unsigned int n = 0; n < pts.size(); n++)
  {
    if (std::get<0>(pts[n]) < std::get<0>(xmin))
      xmin = pts[n];
    if (std::get<0>(pts[n]) > std::get<0>(xmax))
      xmax = pts[n];
    if (std::get<1>(pts[n]) < std::get<1>(ymin))
      ymin = pts[n];
    if (std::get<1>(pts[n]) > std::get<1>(ymax))
      ymax = pts[n];
    if (std::get<2>(pts[n]) < std::get<2>(zmin))
      zmin = pts[n];
    if (std::get<2>(pts[n]) > std::get<2>(zmax))
      zmax = pts[n];
  }

  xSpan = std::pow(std::get<0>(xmax) - std::get<0>(xmin), 2.f) + std::pow(std::get<1>(xmax) - std::get<1>(xmin), 2.f) + std::pow(std::get<2>(xmax) - std::get<2>(xmin), 2.f);
  ySpan = std::pow(std::get<0>(ymax) - std::get<0>(ymin), 2.f) + std::pow(std::get<1>(ymax) - std::get<1>(ymin), 2.f) + std::pow(std::get<2>(ymax) - std::get<2>(ymin), 2.f);
  zSpan = std::pow(std::get<0>(zmax) - std::get<0>(zmin), 2.f) + std::pow(std::get<1>(zmax) - std::get<1>(zmin), 2.f) + std::pow(std::get<2>(zmax) - std::get<2>(zmin), 2.f);

  dia1 = xmin;
  dia2 = xmax;

  maxSpan = xSpan;

  if (ySpan > maxSpan)
  {
    maxSpan = ySpan;
    dia1 = ymin;
    dia2 = ymax;
  }
  if (zSpan > maxSpan)
  {
    maxSpan = zSpan;
    dia1 = zmin;
    dia2 = zmax;
  }

  std::get<0>(center) = (std::get<0>(dia1) + std::get<0>(dia2)) / 2.f;
  std::get<1>(center) = (std::get<1>(dia1) + std::get<1>(dia2)) / 2.f;
  std::get<2>(center) = (std::get<2>(dia1) + std::get<2>(dia2)) / 2.f;

  sqRad = std::pow(std::get<0>(dia2) - std::get<0>(center), 2.f) + std::pow(std::get<1>(dia2) - std::get<1>(center), 2.f) + std::pow(std::get<2>(dia2) - std::get<2>(center), 2.f);
  radius = std::sqrt(sqRad);

  for (unsigned int n = 0; n < pts.size(); n++)
  {
    double	d;

    d = std::pow(std::get<0>(pts[n]) - std::get<0>(center), 2.f) + std::pow(std::get<1>(pts[n]) - std::get<1>(center), 2.f) + std::pow(std::get<2>(pts[n]) - std::get<2>(center), 2.f);

    if (d > sqRad)
    {
      double	r, offset;

      r = std::sqrt(d);
      radius = (radius + r) / 2.f;
      sqRad = radius * radius;
      offset = r - radius;

      std::get<0>(center) = (radius * std::get<0>(center) + offset * std::get<0>(pts[n])) / r;
      std::get<1>(center) = (radius * std::get<1>(center) + offset * std::get<1>(pts[n])) / r;
      std::get<2>(center) = (radius * std::get<2>(center) + offset * std::get<2>(pts[n])) / r;
    }
  }

  RT::AbstractCsgNode *  node = new RT::TransformationCsgNode(Math::Matrix<4, 4>::translation(std::get<0>(center), std::get<1>(center), std::get<2>(center)));
  node->push(new RT::SphereCsgLeaf(radius));

  return node;
}

double		Math::Random::_table[Math::RandomTableSize];
unsigned int	Math::Random::_cursor = 0;

void	Math::Random::initialize()
{
  // Set random generator to timestamp
  std::srand((unsigned int)std::time(nullptr));

  // Fill random table
  for (unsigned int i = 0; i < Math::RandomTableSize; i++)
    _table[i] = (double)std::rand() / (double)RAND_MAX;
}
