#pragma once
#include <vector>
#include "Geometry.h"
#include "Light.h"
#include "Camera.h"
#include "Ray.h"
#include "HitRecord.h"
#include <Windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

class Scene
{
public:
	Scene(void);
	Scene(std::istream&);
	~Scene(void);
	void toStream(std::ostream&);
	Bitmap* trace(int);
	void addElement(Geometry*);
	void addElement(Camera*);
	void addElement(Light*);
	int width, height;
	HitRecord getHitRecord(Ray r);
private:
	std::vector<Geometry*> geom;
	std::vector<Light*> lights;
	std::vector<Camera*> cameras;
	Ray getRay(int, int, int);
};

