#pragma once

#include "Vec3d.h"
#include "Scene.h"

namespace photon{
	enum Progress {DESTROY, CONTINUE, HIT};
}

class Photon
{
public:
	Photon(void);
	~Photon(void);
	photon::Progress tick(double, Scene*);
	Vec3d position, destination;
};



