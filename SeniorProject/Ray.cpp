#include "stdafx.h"
#include "Ray.h"


Ray::Ray(void)
{
}


Ray::~Ray(void)
{
}

Ray::Ray(Vec3d start, Vec3d end){
	this->origin = start;
	this->direction = (end - start).normal();
}

Vec3d Ray::pointAt(double d){
	Vec3d point = origin;
	point = point + (direction * d);
	return point;
}

double Ray::dot(Ray other){
	return direction.x * other.direction.x + direction.y * other.direction.y + direction.z * other.direction.z;
}