#ifndef _MESH_CSG_NODE_HPP_
#define _MESH_CSG_NODE_HPP_

#include <list>
#include <map>
#include <string>
#include <tuple>

#include "AbstractCsgNode.hpp"

namespace RT
{
  class MeshCsgNode : public RT::AbstractCsgNode
  {
  private:
    // Copright (C) 1999-2013, Bernd Gaertner
    // http://www.inf.ethz.ch/personal/gaertner
    // GNU General Public License (GPLv3)
    class BoundingSphere
    {
    private:
      // Variables used for computation
      std::list<std::tuple<double, double, double>>	_support;
      std::tuple<double, double, double>		_q0, _a[4], _c[4], _v[4];
      double						_f[4], _z[4], _s[4];
      int						_fsize = 0, _ssize = 0;

      // Methods used for computation
      void	mtf_mb(std::list<std::tuple<double, double, double>>);
      bool	push(std::tuple<double, double, double>);

    public:
      BoundingSphere(std::list<std::tuple<double, double, double>>);

      std::tuple<double, double, double>	center;	// Center coordinates (x, y, z)
      double					radius;	// Squared radius
    };

    static std::map<std::string, void(RT::MeshCsgNode::*)(std::string const &)> const	_extension;

    double	_x, _y, _z, _r;				// Bounding sphere coordinates/raduis

    void	loadStl(std::string const &);		// Load a .stl file

    void	bound();				// Compute bounding sphere from children
    
  public:
    MeshCsgNode();
    MeshCsgNode(std::string const &);
    ~MeshCsgNode();

    std::list<RT::Intersection>	render(RT::Ray const &, unsigned int) const override;	// Render sub-tree

    void	push(RT::AbstractCsgTree *) override;	// Push a triangle in mesh
    static bool	extension(std::string const &);		// Return true if extension supported
  };
};

#endif
