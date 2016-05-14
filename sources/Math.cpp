#include <random>

#ifdef _DEBUG
#include <stdexcept>
#endif

#include "Math.hpp"

void	Math::initialize()
{
  // Initialize random module
  Math::Random::initialize();
}

std::vector<double>	Math::solve(double a, double b)
{
#ifdef _DEBUG
  // Check for invalid parameter
  if (a == 0)
    return std::vector<double>();
#endif

  // Solve equation
  return { -b / a };
}

std::vector<double>	Math::solve(double a, double b, double c)
{
#ifdef _DEBUG
  // Check for invalid parameter
  if (a == 0)
    return std::vector<double>();
#endif

  // Solve equation
  double		d = b * b - 4.f * a * c;
  if (d >= 0)
    return{ (-b - std::sqrt(d)) / (2.f * a), (-b + std::sqrt(d)) / (2.f * a) };
  else
    return std::vector<double>();
}

std::vector<double>	Math::solve(double a, double b, double c, double d)
{
#ifdef _DEBUG
  // Check for invalid parameter
  if (a == 0)
    return std::vector<double>();
#endif

  b /= a;
  c /= a;
  d /= a;

  double		q = (b * b - 3.f * c) / 9.f;
  double		r = (b * (b * b - 4.5f * c) + 13.5f * d) / 27.f;
  double		delta = q * q * q - r * r;

  if (delta >= 0.f)
  {
    double		phi = std::acos(r / std::pow(q, 3.f / 2.f)) / 3.f;
    double		s = -2.f * std::sqrt(q);

    return{ s * std::cos(phi) - b / 3.f, s * std::cos(phi + 2.f * Math::Pi / 3.f) - b / 3.f, s * std::cos(phi + 4.f * Math::Pi / 3.f) - b / 3.f };
  }
  else
  {
    double		s = std::cbrt((std::sqrt(-delta) + std::abs(r)));

    if (r < 0.f)
      return{ +s + q / s - b / 3.f };
    else
      return{ -s - q / s - b / 3.f };
  }
}

std::vector<double>	Math::solve(double a, double b, double c, double d, double e)
{
#ifdef _DEBUG
  // Check for invalid parameter
  if (a == 0)
    return std::vector<double>();
#endif

  // Solve equation
  double		delta = b / (2.f * a);

  double		f = c / a - 3.f * std::pow(delta, 2) / 2.f;
  double		g = d / a + std::pow(delta, 3) - c * delta / a;
  double		h = e / a - 3.f * std::pow(delta, 4) / 16.f + c * std::pow(delta, 2) / (4.f * a) - d * delta / (2.f * a);
  double		i = -2.f * std::pow(f, 3) / 27.f - std::pow(g, 2) + 8.f * f * h / 3.f;
  double		j = -(std::pow(f, 2) + 12.f * h) / 3.f;

  delta = std::pow(j, 3) / 27.f + std::pow(i, 2) / 4.f;
  if (delta > 0)
    delta = std::cbrt(-i / 2.f + std::sqrt(delta)) - (j / 3.f) / std::cbrt(-i / 2.f + std::sqrt(delta));
  else if (delta == 0)
    delta = 3.f * i / j;
  else
    delta = 2.f * std::sqrt(-j / 3.f) * std::cos(std::acos(-i / 2.f / std::pow((-j / 3.f), (3.f / 2.f))) / 3.f);

  double		k = f / 3.f + delta;
  double		l = std::sqrt(k - f);
  double		m = std::sqrt(std::pow(k / 2.f, 2) - h);
  double		n = -b / (4.f * a);

  delta = std::pow(l, 2) - 2.f * k + 4.f * m;
  if (delta < 0.f)
    return std::vector<double>();

  std::vector<double>	result;

  double		x1 = l / 2.f;
  double		x2 = std::sqrt(delta) / 2.f;
  if (g > 0)
    x1 *= -1.f;

  double	      r0 = x1 - x2 + n;
  double	      r1 = x1 + x2 + n;
  if (!std::isnan(r0) && !std::isnan(r1))
  {
    result.push_back(r0);
    result.push_back(r1);
  }

  delta = std::pow(l, 2) - 2.f * k - 4.f * m;
  if (delta < 0.f)
    return result;

  double		y1 = -l / 2.f;
  double		y2 = std::sqrt(delta) / 2.f;
  if (g > 0)
    y1 *= -1.f;

  double		r2 = y1 - y2 + n;
  double		r3 = y1 + y2 + n;
  if (!std::isnan(r2) && !std::isnan(r3))
  {
    result.push_back(r2);
    result.push_back(r3);
  }

  return result;
}

double		Math::Random::_table[Math::RandomTableSize];
unsigned int	Math::Random::_cursor = 0;

void		Math::Random::initialize()
{
  // Set random generator to timestamp
  std::default_random_engine			generator;
  std::uniform_real_distribution<double>	distribution(0.f, 1.f);

  // Fill random table
  for (unsigned int i = 0; i < Math::RandomTableSize; i++)
    _table[i] = distribution(generator);
}
