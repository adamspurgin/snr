#include "stdafx.h"
#include "Scene.h"
#include "Utils.h"
#include "Material.h"
#include "Geometry.h"
#include "Sphere.h"
#include <gdiplus.h>

using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")

#define PROGRESS_TICKS 75



Scene::Scene(void)
{
}

Scene::Scene(std::istream& in){
	DEBUG();
	std::istream *stripped = &in;//util::stripSpaces(in);
	DEBUG();
	util::match(in, "{type:SCN,width:");
	DEBUG();
	*stripped >> width;
	util::match(in, ",height:");
	*stripped >> height;
	util::match(*stripped, ",cameras:[");
	while((*stripped).get() == '{'){
		(*stripped).unget();
		this->addElement(new Camera(in));
		if((*stripped).get() != ','){
			(*stripped).unget();
		}
	}
	(*stripped).unget();

	util::match((*stripped), "],lights:[");
	while((*stripped).get() == '{'){
		(*stripped).unget();
		this->addElement(new Light(in));
		if((*stripped).get() != ','){
			(*stripped).unget();
		}
	}
	(*stripped).unget();

	util::match((*stripped), "],geometry:[");
	while((*stripped).get() == '{'){
		(*stripped).unget();
		this->addElement(Geometry::fromStream((*stripped)));
		if((*stripped).get() != ','){
			(*stripped).unget();
		}
	}
	(*stripped).unget();
	util::match((*stripped), "]}");
}

Scene::~Scene(void)
{
}

void Scene::toStream(std::ostream& str){
	str << "{type:SCN,width:" << width << ",height:"<<height<<",";
	str << "cameras:[";
	for(unsigned int i=0; i < cameras.size(); i++){
		cameras[i]->toStream(str);
		if(i != cameras.size() - 1){
			str << ",";
		}
	}
	str << "],lights:[";
	for(unsigned int i=0; i < lights.size(); i++){
		lights[i]->toStream(str);
		if(i != lights.size() - 1){
			str << ",";
		}
	}
	str << "],geometry:[";
	for(unsigned int i=0; i < geom.size(); i++){
		geom[i]->toStream(str);
		if(i != geom.size() - 1){
			str << ",";
		}
	}
	str << "]}";
}

void Scene::addElement(Geometry* g){
	geom.push_back(g);
}

void Scene::addElement(Light* l){
	lights.push_back(l);
}

void Scene::addElement(Camera* l){
	cameras.push_back(l);
}

HitRecord Scene::getHitRecord(Ray r, bool getLight){
	long distance = LONG_MAX;
	HitRecord hr;
	for(unsigned int g=0; g<geom.size(); g++){
		long intersect = geom[g]->intersect(r);

		if(intersect > 0 && intersect < distance){
			hr.hitObject = geom[g];
			hr.distance = intersect;
			hr.hit = true;
			hr.location = r.pointAt(intersect);
			hr.hitNormal = geom[g]->normalAt2(hr.location);
			if(getLight){
				DEBUG();
				Vec3d light = getLightAt(r.pointAt(intersect -0.000001), geom[g], r, hr.hitNormal);
				hr.color.x = light.x * geom[g]->material.getRed();
				hr.color.y = light.y * geom[g]->material.getBlue();
				hr.color.z = light.z * geom[g]->material.getGreen();
			}
		}
	}
	return hr;
}

Vec3d Scene::getLightAt(Vec3d position, Geometry* object, Ray incoming, Vec3d hitNormal){
	Vec3d lightVal(1,1,1);
	for(unsigned int i = 0; i < lights.size(); i++){
		Ray r(position, lights[i]->position);
		HitRecord hr = getHitRecord(r, false);
		if(!hr.hit || hr.distance < position.distanceTo(lights[i]->position)){
			DEBUG();
			double diffuse = hitNormal.dot(r.direction);
			diffuse = (diffuse >= 0) ? ((diffuse >= 1)? 1 : diffuse) : 0.01;
			/*hitNormal.toStream(std::cout);
			std::cout << ", ";
			r.direction.toStream(std::cout);
			std::cout << ": ";
			std::cout << hitNormal.dot(r.direction) << "\n";*/
			lightVal.x += (diffuse) / lights.size();
			lightVal.y += (diffuse) / lights.size();
			lightVal.z += (diffuse) / lights.size();
		}
	}
	return lightVal;
}

Bitmap* Scene::trace(int cam, double tick){
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	DEBUG();
	Bitmap* b = new Bitmap(width, height);

	std::cout << "|";
	for(int i=0; i<PROGRESS_TICKS - 2; i++){
		std::cout << "-";
	}
	std::cout << "|\n";
	
	int last = 0;

	setTick(tick);
	DEBUG();
	//trace loop
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			int current = ((double)(i*width + j) / ((double)width*(double)height)) * PROGRESS_TICKS;
			Ray r = getRay(i, j, cam);
			HitRecord hr = getHitRecord(r, true);
			if(hr.hit){
				b->SetPixel(j, i, Gdiplus::Color(hr.color.x * 255, hr.color.y * 255, hr.color.z * 255));
			} else {
				b->SetPixel(j, i, Gdiplus::Color(0, 0, 0));
			}
			if(current > last){
				std::cout << "=";
				last = current;
			}
		}
	}
	std::cout << "=\n";

	return b;
}

void Scene::setTick(double tick){
	for(unsigned int g = 0; g < geom.size(); g++){
		geom[g]->setTick(tick);
	}
}


Ray Scene::getRay(int row, int col, int cam){
	Camera c = *cameras[cam];
	Vec3d start = c.position;
	double tick = c.focalWidth / width;
	Vec3d end;
	end.x = c.focalLength;
	end.y = (-row + height/2) * tick;
	end.z = (-col + width/2) * tick;
	end = end + start;
	return Ray(start, end);
}