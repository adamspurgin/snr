#pragma once
#include "Vec3d.h"

class Ray
{
public:
	Ray();
	Ray(Vec3d, Vec3d);
	virtual ~Ray();
	Vec3d pointAt(double);
	Vec3d origin;
	Vec3d direction;
};

