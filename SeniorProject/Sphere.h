#pragma once
#include "geometry.h"
#include "Vec3d.h"

class Sphere :
	public Geometry
{
public:
	Sphere(void);
	Sphere(std::istream&);
	~Sphere(void);
	virtual void toStream(std::ostream&);
	virtual double intersect(Ray);
	Vec3d center;
	double radius;
};

