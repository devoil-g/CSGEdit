#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <iostream>
#include <Eigen/Eigen>

#include "Math.hpp"

namespace Math
{

  template<unsigned int cCol, unsigned int cRow>
  class Matrix
  {
  public:
    Eigen::Matrix<double, cRow, cCol>	_matrix;
    
    Matrix(Eigen::Matrix<double, cRow, cCol> const & matrix) : _matrix(matrix) {};

  public:
    Matrix() : _matrix(Eigen::Matrix<double, cRow, cCol>::Zero()) {};
    ~Matrix() {};

    inline double &			operator()(unsigned int col, unsigned int row)
    {
#ifdef _DEBUG
      if (col >= cCol || row >= cRow)
	throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

      return _matrix(row, col);
    };
    
    inline double			operator()(unsigned int col, unsigned int row) const
    {
#ifdef _DEBUG
      if (col >= cCol || row >= cRow)
	throw RT::Exception(std::string(__FILE__) + ": l." + std::to_string(__LINE__));
#endif

      return _matrix(row, col);
    };

    template<unsigned int dCol, unsigned int dRow>
    inline Math::Matrix<dCol, cRow>	operator*(Math::Matrix<dCol, dRow> const & right) const
    {
      return Math::Matrix<dCol, cRow>(_matrix * right._matrix);
    }

    inline Math::Matrix<cCol, cRow>	inverse() const
    {
      return Math::Matrix<cCol, cRow>(_matrix.inverse());
    }

    inline Math::Matrix<cCol, cRow>	transpose() const
    {
      return Math::Matrix<cCol, cRow>(_matrix.transpose());
    }

    inline static Math::Matrix<cCol, cRow>	identite()
    {
      return Math::Matrix<cCol, cRow>(Eigen::Matrix<double, cRow, cCol>::Identity());
    }

    inline static Math::Matrix<4, 4>	translation(double x, double y, double z)
    {
      return Math::Matrix<4, 4>(Eigen::Affine3d(Eigen::Translation3d(Eigen::Vector3d(x, y, z))).matrix());
    }

    inline static Math::Matrix<4, 4>	rotation(double x, double y, double z)
    {
      return Math::Matrix<4, 4>((Eigen::Affine3d(Eigen::AngleAxisd(Math::Utils::DegToRad(z), Eigen::Vector3d(0, 0, 1))) * Eigen::Affine3d(Eigen::AngleAxisd(Math::Utils::DegToRad(y), Eigen::Vector3d(0, 1, 0))) * Eigen::Affine3d(Eigen::AngleAxisd(Math::Utils::DegToRad(x), Eigen::Vector3d(1, 0, 0)))).matrix());
    }

    inline static Math::Matrix<4, 4>	rotation(double a, double x, double y, double z)
    {
      return Math::Matrix<4, 4>((Eigen::Affine3d(Eigen::AngleAxisd(Math::Utils::DegToRad(a), Eigen::Vector3d(x, y, z)))).matrix());
    }

    inline static Math::Matrix<4, 4>	scale(double x, double y, double z)
    {
      return Math::Matrix<4, 4>(Eigen::Affine3d(Eigen::Scaling(x, y, z)).matrix());
    }

    inline static Math::Matrix<4, 4>	scale(double s)
    {
      return Math::Matrix<4, 4>(Eigen::Affine3d(Eigen::Scaling(s)).matrix());
    }
  };
};

#endif