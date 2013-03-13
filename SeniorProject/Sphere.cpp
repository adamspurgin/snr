#include "stdafx.h"
#include "Sphere.h"
#include "Utils.h"


Sphere::Sphere(void)
{
	radius = 0;
}

Sphere::Sphere(std::istream& in){
	util::match(in, "{type:sphere,loc:");
	center = Vec3d(in);
	util::match(in, ",radius:");
	in >> radius;
	util::match(in, ",material:");
	material = Material(in);
	util::match(in, "}");
}

Sphere::~Sphere(void)
{
}

void Sphere::toStream(std::ostream& str){
	str << "{type:sphere,loc:";
	center.toStream(str);
	str << ",radius:" << radius << ",material:";
	material.toStream(str);
	str << "}";
}

double Sphere::intersect(Ray r){
	double A = r.direction.dot(r.direction);
	double B = 2*(r.direction.dot(r.origin - center));
	double C = (r.origin - center).dot(r.origin - center) - radius*radius;
	double r1, r2;
	if(!util::quadRoots(A, B, C, &r1, &r2)){
		return -1;
	} else if(r2 >= 0 && r1 > r2) {
		return r2;
	} else if(r1 >= 0 && r1 < r2) {
		return r1;
	} else {
		return -1;
	}
}