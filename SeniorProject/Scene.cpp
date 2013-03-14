#include "stdafx.h"
#include "Scene.h"
#include "Utils.h"
#include <gdiplus.h>

using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")

#define PROGRESS_TICKS 75

Scene::Scene(void)
{
}

Scene::Scene(std::istream& in){
	util::match(in, "{type:SCN,width:");
	in >> width;
	util::match(in, ",height:");
	in >> height;
	util::match(in, ",cameras:[");
	while(in.get() == '{'){
		in.unget();
		this->addElement(new Camera(in));
		if(in.get() != ','){
			in.unget();
		}
	}
	in.unget();

	util::match(in, "],lights:[");
	while(in.get() == '{'){
		in.unget();
		this->addElement(new Light(in));
		if(in.get() != ','){
			in.unget();
		}
	}
	in.unget();

	util::match(in, "],geometry:[");
	while(in.get() == '{'){
		in.unget();
		this->addElement(Geometry::fromStream(in));
		if(in.get() != ','){
			in.unget();
		}
	}
	in.unget();
	util::match(in, "]}");
	//TODO:(the rest of this)
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

HitRecord Scene::getHitRecord(Ray r){
	long distance = LONG_MAX;
	HitRecord hr;
	for(int g=0; g<geom.size(); g++){
		long intersect = geom[g]->intersect(r);
		if(intersect > 0 && intersect < distance){
			hr.hitObject = geom[g];
			hr.distance = intersect;
			hr.hit = true;
		}
	}
	return hr;
}

Bitmap* Scene::trace(int cam){
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	Bitmap* b = new Bitmap(width, height);

	std::cout << "|";
	for(int i=0; i<PROGRESS_TICKS - 2; i++){
		std::cout << "-";
	}
	std::cout << "|\n";
	
	int last = 0;
	//trace loop
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			int current = ((double)(i*width + j) / ((double)width*(double)height)) * PROGRESS_TICKS;
			Ray r = getRay(i, j, cam);
			HitRecord hr = this->getHitRecord(r);
			if(hr.hit){
				b->SetPixel(j, i, Gdiplus::Color(255, 255, 255));
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