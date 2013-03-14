#pragma once
#include "Geometry.h"

class HitRecord
{
public:
	HitRecord(void);
	~HitRecord(void);
	Geometry* hitObject;
	Vec3d location;
	double distance;
	bool hit;
};

