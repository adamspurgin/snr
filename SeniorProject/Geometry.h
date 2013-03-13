#pragma once
#include "SceneElement.h"
#include "Ray.h"
#include "Material.h"
#include <iostream>




class Geometry :
	public SceneElement
{
public:
	enum geomTypes {SPHERE, OTHER};
	static geomTypes getNextType(std::istream&);
	Geometry(void);
	static Geometry* fromStream(std::istream&);
	~Geometry(void);
	Material material;
	virtual double intersect(Ray);
	virtual void toStream(std::ostream&);
};

