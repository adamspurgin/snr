#include "stdafx.h"
#include "Geometry.h"
#include "Sphere.h"
#include <iostream>


Geometry::Geometry(void)
{
}


Geometry::~Geometry(void)
{
}

Geometry* Geometry::fromStream(std::istream &in){
	switch(getNextType(in)){
	case SPHERE:
		//std::cout << "SPHERE\n";
		return new Sphere(in);
		break;
	case OTHER:
		//std::cout << "OTHER\n";
		return new Geometry();
		break;
	default:
		//std::cout << "ERR\n";
		return new Geometry();
		break;
	}
}

Geometry::geomTypes Geometry::getNextType(std::istream& in){
	long loc = in.tellg();
	char buffer[30];
	in.get(buffer, 29, ',');
	//std::cout << "--------" << buffer << "--------\n";
	if(strcmp(buffer, "{type:sphere") == 0) {
		in.seekg(loc, std::ios_base::beg);
		return SPHERE;
	} else {
		in.seekg(loc, std::ios_base::beg);
		return OTHER;
	}
}

void Geometry::toStream(std::ostream& str){
	str << "test";
}

double Geometry::intersect(Ray r){
	return -1;
}
