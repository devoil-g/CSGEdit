#ifndef _OPENCL_STRUCTURES_HPP_
#define _OPENCL_STRUCTURES_HPP_

#include <vector>

#pragma pack(push, 1)

namespace RT
{
  namespace OpenCL
  {
    struct Node
    {
      enum Type : int
      {
	TypeUnion = 0,
	TypeDifference = 1,
	TypeIntersection = 2,
	TypePrimitive = 3,
	TypeEmpty = 4
      };

      struct Csg
      {
	int	left;	// Index of left node
	int	right;	// Index of right node
      };

      struct Primitive
      {
	int	index;	// Index of primitive
      };

      union Data
      {
	RT::OpenCL::Node::Csg		csg;
	RT::OpenCL::Node::Primitive	primitive;
      };

      RT::OpenCL::Node::Type	type;	// Type of node
      RT::OpenCL::Node::Data	data;	// Data of node
    };

    struct Primitive
    {
      enum Type : int
      {
	PrimitiveBox = 0,
	PrimitiveCone = 1,
	PrimitiveSphere = 2,
	PrimitiveTangle = 3,
	PrimitiveTorus = 4,
	PrimitiveTriangle = 5
      };

      struct Box
      {};

      struct Cone
      {
	float	r1;	// Bottom radius
	float	r2;	// Top radius
      };

      struct Mesh
      {
	float	x, y, z;	// Coordinates of bounding sphere
	float	r;		// Radius of bounding box
	int	index;		// Index of first triangle
      };

      struct Sphere
      {};

      struct Tangle
      {
	float	c;	// Tangle constant
      };

      struct Torus
      {
	float	r;	// Small radius
      };

      struct Triangle
      {
	float	p[3][3];	// Triangle points
	float	v[2][3];	// Pre-computed vectors
	float	n[3];		// Normal
	int	index;		// Index of next triangle, 0 if last
      };

      struct Material
      {
	float	color[3];
      };

      union Data
      {
	RT::OpenCL::Primitive::Box	box;
	RT::OpenCL::Primitive::Cone	cone;
	RT::OpenCL::Primitive::Sphere	sphere;
	RT::OpenCL::Primitive::Tangle	tangle;
	RT::OpenCL::Primitive::Torus	torus;
	RT::OpenCL::Primitive::Triangle	triangle;
      };
      
      RT::OpenCL::Primitive::Type	type;			// Type of primitive
      float				transformation[16];	// Transformation matrix of primitive
      RT::OpenCL::Primitive::Material	material;		// Material of primitive
      RT::OpenCL::Primitive::Data	data;			// Data of primitive
    };

    void	normalize(std::vector<RT::OpenCL::Node> &, std::vector<RT::OpenCL::Primitive> &);
  };
};

#pragma pack(pop)

#endif
