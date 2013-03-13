#pragma once
#include "sceneelement.h"
#include "Vec3d.h"

#define CAM_START "{CAM:"
#define CAM_END ":CAM}"

class Camera :
	public SceneElement
{
public:
	Camera(void);
	Camera(std::istream&);
	~Camera(void);
	Vec3d position;
	double focalLength, focalWidth;
	Vec3d rotation;
	virtual void toStream(std::ostream&);
};

