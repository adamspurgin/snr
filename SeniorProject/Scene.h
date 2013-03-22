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
	Bitmap* trace(int, double);
	void addElement(Geometry*);
	void addElement(Camera*);
	void addElement(Light*);
	int width, height;
	HitRecord getHitRecord(Ray, bool);
private:
	std::vector<Geometry*> geom;
	std::vector<Light*> lights;
	std::vector<Camera*> cameras;
	Ray getRay(int, int, int);
	Vec3d getLightAt(Vec3d, Geometry*, Ray, Vec3d);
	void setTick(double);
};

