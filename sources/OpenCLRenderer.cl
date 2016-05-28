
#define	PrimitiveBox		0
#define	PrimitiveCone		1
#define	PrimitiveSphere		2
#define	PrimitiveTangle		3
#define	PrimitiveTorus		4
#define	PrimitiveTriangle	5

struct __attribute__ ((packed)) Box
{};

struct __attribute__ ((packed)) Cone
{
	float	r1;			// Bottom radius
	float	r2;			// Top radius
};

struct __attribute__ ((packed)) Mesh
{
	float	x, y, z;	// Coordinates of bounding sphere
	float	r;			// Radius of bounding box
	int		index;		// Index of first triangle
};

struct __attribute__ ((packed)) Sphere
{};

struct __attribute__ ((packed)) Tangle
{
	float	c;			// Tangle constant
};

struct __attribute__ ((packed)) Torus
{
	float	r;			// Small radius
};

struct __attribute__ ((packed)) Triangle
{
	float	p[3][3];	// Triangle points
	float	v[2][3];	// Pre-computed vectors
	float	n[3];		// Normal
	int		index;		// Index of next triangle, 0 if last
};

struct __attribute__ ((packed)) Material
{
	float	color[3];
};

union Data
{
	struct Box		box;
	struct Cone		cone;
	struct Sphere	sphere;
	struct Tangle	tangle;
	struct Torus	torus;
	struct Triangle	triangle;
};

struct __attribute__ ((packed)) Primitive
{
	int				type;				// Type of primitive
	float			transformation[16];	// Transformation matrix of primitive
	struct Material	material;			// Material of primitive
	union Data		data;				// Data of primitive
};

struct __attribute__ ((packed)) Ray
{
	float	p[4];	// Position
	float	d[4];	// Direction
};

void			CrossG31x31(__global float * m1, float * m2, float * result)
{
	float		tmp[3];

	tmp[0] = m1[1] * m2[2] - m1[2] * m2[1];
	tmp[1] = m1[2] * m2[0] - m1[0] * m2[2];
	tmp[2] = m1[0] * m2[1] - m1[1] * m2[0];

	for (unsigned int x = 0; x < 3; x++)
		result[x] = tmp[x];
}

void			Cross31xG31(float * m1, __global float * m2, float * result)
{
	float		tmp[3];

	tmp[0] = m1[1] * m2[2] - m1[2] * m2[1];
	tmp[1] = m1[2] * m2[0] - m1[0] * m2[2];
	tmp[2] = m1[0] * m2[1] - m1[1] * m2[0];

	for (unsigned int x = 0; x < 3; x++)
		result[x] = tmp[x];
}

void			MultiplyG44x41(__global float * m1, float * m2, float * result)
{
	float		tmp[4];

	for (unsigned int y = 0; y < 4; y++)
	{
		tmp[y] = 0.f;
		for (unsigned int x = 0; x < 4; x++)
			tmp[y] += m1[y * 4 + x] * m2[x];
	}

	for (unsigned int x = 0; x < 4; x++)
		result[x] = tmp[x];
}

float			IntersectionBox(__global struct Primitive * primitive, struct Ray * ray)
{
	float		result[6];

	result[0] = 0.f;
	result[1] = 0.f;
	result[2] = 0.f;
	result[3] = 0.f;
	result[4] = 0.f;
	result[5] = 0.f;
	
	// Intersection with X faces
	if (ray->d[0] != 0.f)
	{
		// Calculate intersection distance
		float	x1 = (-ray->p[0]) / ray->d[0];
		float	x2 = (1.f - ray->p[0]) / ray->d[0];

		// Push intersections if inside faces
		if (ray->p[1] + x1 * ray->d[1] >= 0.f &&
			ray->p[1] + x1 * ray->d[1] < 1.f &&
			ray->p[2] + x1 * ray->d[2] >= 0.f &&
			ray->p[2] + x1 * ray->d[2] < 1.f)
			result[0] = x1;
		if (ray->p[1] + x2 * ray->d[1] >= 0.f &&
			ray->p[1] + x2 * ray->d[1] < 1.f &&
			ray->p[2] + x2 * ray->d[2] >= 0.f &&
			ray->p[2] + x2 * ray->d[2] < 1.f)
			result[1] = x2;
	}
	
	// Intersection with Y faces
	if (ray->d[1] != 0.f)
	{
		// Calculate intersection distance
		float	y1 = (-ray->p[1]) / ray->d[1];
		float	y2 = (1.f - ray->p[1]) / ray->d[1];

		// Push intersections if inside faces
		if (ray->p[0] + y1 * ray->d[0] >= 0.f &&
			ray->p[0] + y1 * ray->d[0] < 1.f &&
			ray->p[2] + y1 * ray->d[2] >= 0.f &&
			ray->p[2] + y1 * ray->d[2] < 1.f)
			result[2] = y1;
		if (ray->p[0] + y2 * ray->d[0] >= 0.f &&
			ray->p[0] + y2 * ray->d[0] < 1.f &&
			ray->p[2] + y2 * ray->d[2] >= 0.f &&
			ray->p[2] + y2 * ray->d[2] < 1.f)
			result[3] = y2;
	}

	// Intersection with Z faces
	if (ray->d[2] != 0.f)
	{
		// Calculate intersection distance
		float	z1 = (-ray->p[2]) / ray->d[2];
		float	z2 = (1.f - ray->p[2]) / ray->d[2];

		// Push intersections if inside faces
		if (ray->p[0] + z1 * ray->d[0] >= 0.f &&
			ray->p[0] + z1 * ray->d[0] < 1.f &&
			ray->p[1] + z1 * ray->d[1] >= 0.f &&
			ray->p[1] + z1 * ray->d[1] < 1.f)
			result[4] = z1;
		if (ray->p[0] + z2 * ray->d[0] >= 0.f &&
			ray->p[0] + z2 * ray->d[0] < 1.f &&
			ray->p[1] + z2 * ray->d[1] >= 0.f &&
			ray->p[1] + z2 * ray->d[1] < 1.f)
			result[5] = z2;
	}

	// Get smaller result
	float	ret = 0.f;

	for (unsigned int i = 0; i < 6; i++)
		if (result[i] > 0.f && (ret == 0.f || result[i] < ret))
			ret = result[i];

	return ret;
}

float			IntersectionCone(__global struct Primitive * primitive, struct Ray * ray)
{
	float		result[6];

	result[0] = 0.f;
	result[1] = 0.f;
	result[2] = 0.f;
	result[3] = 0.f;
	
	float		a = pow(ray->d[0], 2) + pow(ray->d[1], 2) - pow(ray->d[2] * (primitive->data.cone.r2 - primitive->data.cone.r1), 2);
    float		b = 2.f * (ray->p[0] * ray->d[0] + ray->p[1] * ray->d[1] - ray->p[2] * ray->d[2] * pow(primitive->data.cone.r2 - primitive->data.cone.r1, 2) - ray->d[2] * primitive->data.cone.r1 * (primitive->data.cone.r2 - primitive->data.cone.r1));
    float		c = pow(ray->p[0], 2) + pow(ray->p[1], 2) - pow(ray->p[2] * (primitive->data.cone.r2 - primitive->data.cone.r1), 2) - 2.f * ray->p[2] * primitive->data.cone.r1 * (primitive->data.cone.r2 - primitive->data.cone.r1) - pow(primitive->data.cone.r1, 2);

	float		d = pow(b, 2) - 4.f * a * c;

	if (d > 0.f)
	{
		float	x;

		x = (-b - sqrt(d)) / (2 * a);
		if (ray->p[2] + x * ray->d[2] >= 0.f && ray->p[2] + x * ray->d[2] <= 1.f)
			result[0] = x;

		x = (-b + sqrt(d)) / (2 * a);
		if (ray->p[2] + x * ray->d[2] >= 0.f && ray->p[2] + x * ray->d[2] <= 1.f)
			result[1] = x;
	}

	if (ray->d[2] != 0.f)
	{
		float	x1 = (-ray->p[2]) / ray->d[2];
		float	x2 = (1.f - ray->p[2]) / ray->d[2];

		if (pow(ray->p[0] + x1 * ray->d[0], 2) + pow(ray->p[1] + x1 * ray->d[1], 2) < pow(primitive->data.cone.r1, 2))
			result[2] = x1;
		if (pow(ray->p[0] + x2 * ray->d[0], 2) + pow(ray->p[1] + x2 * ray->d[1], 2) < pow(primitive->data.cone.r2, 2))
			result[3] = x2;
	}

	// Get smaller result
	float	ret = 0.f;

	for (unsigned int i = 0; i < 4; i++)
		if (result[i] > 0.f && (ret == 0.f || result[i] < ret))
			ret = result[i];

	return ret;
}

float			IntersectionSphere(__global struct Primitive * primitive, struct Ray * ray)
{
	float		a = pow(ray->d[0], 2) + pow(ray->d[1], 2) + pow(ray->d[2], 2);
	float		b = 2.f * (ray->p[0] * ray->d[0] + ray->p[1] * ray->d[1] + ray->p[2] * ray->d[2]);
	float		c = pow(ray->p[0], 2) + pow(ray->p[1], 2) + pow(ray->p[2], 2) - 1.f;
	
	float		d = pow(b, 2) - 4.f * a * c;

	if (d > 0.f)
	{
		float	x;

		x = (-b - sqrt(d)) / (2 * a);
		if (x > 0.f)
			return x;

		x = (-b + sqrt(d)) / (2 * a);
		if (x > 0.f)
			return x;
	}

	return 0.f;
}

float			IntersectionTangle(__global struct Primitive * primitive, struct Ray * ray)
{
	return 0.f;
}

float			IntersectionTorus(__global struct Primitive * primitive, struct Ray * ray)
{
	return 0.f;
}

float			IntersectionTriangle(__global struct Primitive * primitive, struct Ray * ray)
{
	float		v2[3];
	
	// Very clever method to get intersection distance
	v2[0] = ray->p[0] - primitive->data.triangle.p[0][0];
	v2[1] = ray->p[1] - primitive->data.triangle.p[0][1];
	v2[2] = ray->p[2] - primitive->data.triangle.p[0][2];

	float		d = -(primitive->data.triangle.n[0] * ray->d[0] + primitive->data.triangle.n[1] * ray->d[1] + primitive->data.triangle.n[2] * ray->d[2]);

	float		vec1[3];

	Cross31xG31(v2, primitive->data.triangle.v[1], vec1);

	float		a = -(vec1[0] * ray->d[0] + vec1[1] * ray->d[1] + vec1[2] * ray->d[2]) / d;

	if (a < 0.f || a > 1.f)
		return 0.f;

	float		vec2[3];

	CrossG31x31(primitive->data.triangle.v[0], v2, vec2);
	
	float		b = -(vec2[0] * ray->d[0] + vec2[1] * ray->d[1] + vec2[2] * ray->d[2]) / d;

	// Push intersection if inside the triangle
	if (b > 0.f && a + b < 1.f)
	{
		float	x = (primitive->data.triangle.n[0] * v2[0] + primitive->data.triangle.n[1] * v2[1] + primitive->data.triangle.n[2] * v2[2]) / d;

		if (x > 0.f)
			return x;
	}
	
	return 0.f;
}

float			IntersectionPrimitive(__global struct Primitive * primitive, struct Ray * ray)
{
	struct Ray	r;
	
	MultiplyG44x41(primitive->transformation, ray->p, r.p);
	MultiplyG44x41(primitive->transformation, ray->d, r.d);

	if (primitive->type == PrimitiveBox)
		return IntersectionBox(primitive, &r);
	else if (primitive->type == PrimitiveCone)
		return IntersectionCone(primitive, &r);
	else if (primitive->type == PrimitiveSphere)
		return IntersectionSphere(primitive, &r);
	else if (primitive->type == PrimitiveTangle)
		return IntersectionTangle(primitive, &r);
	else if (primitive->type == PrimitiveTorus)
		return IntersectionTorus(primitive, &r);
	else
		return IntersectionTriangle(primitive, &r);
}

void			CreateRay(__global float * camera, int width, int height, struct Ray * ray)
{
	const int	x = get_global_id(0), y = get_global_id(1);
	
	// Initial position
	ray->p[0] = 0.f;
	ray->p[1] = 0.f;
	ray->p[2] = 0.f;
	ray->p[3] = 1.f;

	// Calcul ray according to (x, y) coordinates
	ray->d[0] = (float)width;
	ray->d[1] = (float)width / 2.f - (float)x + 0.5f;
	ray->d[2] = (float)height / 2.f - (float)y + 0.5f;
	ray->d[3] = 0.f;
	
	// Apply camera transformation
	MultiplyG44x41(camera, ray->p, ray->p);
	MultiplyG44x41(camera, ray->d, ray->d);

	float		l = sqrt(pow(ray->d[0], 2) + pow(ray->d[1], 2) + pow(ray->d[2], 2));

	ray->d[0] /= l;
	ray->d[1] /= l;
	ray->d[2] /= l;
}

__kernel void	PreviewKernel(__global void * nodes, __global struct Primitive * primitives, __global uchar * image, __global float * camera, int width, int height, int n_primitives)
{
	const int	x = get_global_id(0), y = get_global_id(1);
	struct Ray	ray;
	
	// Generate ray
	CreateRay(camera, width, height, &ray);
	
	// Reset pixel value	
	image[(x + y * width) * 4 + 0] = 0;
	image[(x + y * width) * 4 + 1] = 0;
	image[(x + y * width) * 4 + 2] = 0;
	image[(x + y * width) * 4 + 3] = 255;

	float		dist = 0.f;

	for (int n = 0; n < n_primitives; n++)
	{
		float	d = IntersectionPrimitive(&primitives[n], &ray);
		
		if (d != 0.f && (dist == 0.f || d < dist))
			dist = d;
	}

	if (dist > 0.f)
	{
		image[(x + y * width) * 4 + 0] = 255 * (1.f - dist / 1500);
		image[(x + y * width) * 4 + 1] = 255 * (1.f - dist / 1500);
		image[(x + y * width) * 4 + 2] = 255 * (1.f - dist / 1500);
	}
}
 