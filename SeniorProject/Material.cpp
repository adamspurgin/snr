#include "stdafx.h"
#include "Material.h"
#include "Utils.h"

Material::Material(std::istream &in){
	util::match(in, "[");
	in >> r;
	util::match(in, ",");
	in >> g;
	util::match(in, ",");
	in >> b;
	util::match(in, "]");
}

Material::Material(double red, double green, double blue){
	r = red;
	g = green;
	b = blue;
}

Material::Material(void)
{
	r=0;
	g=0;
	b=0;
}


Material::~Material(void)
{
}

void Material::set(double red, double green, double blue){
	r = red;
	g = green;
	b = blue;
}

double Material::getRed(){
	return r;
}

double Material::getGreen(){
	return g;
}

double Material::getBlue(){
	return b;
}

double Material::getHue(){
	return 0;
}

double Material::getSat(){
	return 0;
}

double Material::getVal(){
	return 0;
}

void Material::toStream(std::ostream &strm){
	strm << "[" << r << "," << g << "," << b << "]";
}