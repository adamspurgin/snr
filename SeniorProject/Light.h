#pragma once
#include "SceneElement.h"
#include "Vec3d.h"

class Light :
	public SceneElement
{
public:
	Light(void);
	Light(std::istream&);
	~Light(void);
	virtual void toStream(std::ostream&);
	Vec3d position;
};

