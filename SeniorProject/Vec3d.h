#pragma once

#include <iostream>
class Vec3d
{
public:
	Vec3d(void);
	Vec3d(double, double, double);
	Vec3d(std::istream&);
	~Vec3d(void);
	double x, y, z;
	void normalize();
	Vec3d normal();
	double mag();
	Vec3d operator +(const Vec3d&);
	Vec3d operator -(const Vec3d&);
	Vec3d operator *(const double&);
	Vec3d operator /(const double&);
	double dot(Vec3d);
	void toStream(std::ostream&);
};

