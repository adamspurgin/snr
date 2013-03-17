#pragma once
#include "SceneElement.h"
#include "Ray.h"
#include "Material.h"
#include <iostream>
#include <vector>
#include "KeyFrame.h"
#include "Motion.h"




class Geometry :
	public SceneElement
{
public:
	Motion path;
	enum geomTypes {SPHERE, OTHER};
	static geomTypes getNextType(std::istream&);
	Geometry(void);
	static Geometry* fromStream(std::istream&);
	~Geometry(void);
	Material material;
	virtual double intersect(Ray);
	virtual void toStream(std::ostream&);
};

