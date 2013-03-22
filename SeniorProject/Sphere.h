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
	virtual void setTick(double);
	//virtual Vec3d normalAt(Vec3d);
	virtual Vec3d normalAt2(Vec3d);
	Vec3d center;
	double radius;
};

