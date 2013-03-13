#include "stdafx.h"
#include <iostream>
#include "Vec3d.h"
#include "Utils.h"


Vec3d::Vec3d(void)
{
	x = y = z = 0;
}

Vec3d::Vec3d(std::istream& in){
	util::match(in, "[");
	in >> x;
	util::match(in, ",");
	in >> y;
	util::match(in, ",");
	in >> z;
	util::match(in, "]");
}

Vec3d::Vec3d(double xx, double yy, double zz){
	x=xx;
	y=yy;
	z=zz;
}

Vec3d::~Vec3d(void)
{
}

Vec3d Vec3d::operator +(const Vec3d& rhs){
	Vec3d result;
	result.x = this->x + rhs.x;
	result.y = this->y + rhs.y;
	result.z = this->z + rhs.z;
	return result;
}

Vec3d Vec3d::operator -(const Vec3d& rhs){
	Vec3d result;
	result.x = this->x - rhs.x;
	result.y = this->y - rhs.y;
	result.z = this->z - rhs.z;
	return result;
}

Vec3d Vec3d::operator *(const double& rhs){
	Vec3d result;
	result.x = this->x * rhs;
	result.y = this->y * rhs;
	result.z = this->z * rhs;
	return result;
}

Vec3d Vec3d::operator /(const double& rhs){
	Vec3d result;
	result.x = this->x / rhs;
	result.y = this->y / rhs;
	result.z = this->z / rhs;
	return result;
}

void Vec3d::toStream(std::ostream &str){
	str << "[" << x << "," << y << "," << z << "]";
}

double Vec3d::mag(){
	return sqrt(x*x + y*y + z*z);
}

void Vec3d::normalize(){
	double len = mag();
	x /= len;
	y /= len;
	z /= len;
}

Vec3d Vec3d::normal(){
	Vec3d ret(x, y, z);
	ret.normalize();
	return ret;
}

double Vec3d::dot(Vec3d o){
	return x*o.x + y*o.y + z*o.z;
}