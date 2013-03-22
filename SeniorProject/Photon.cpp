#include "stdafx.h"
#include "Photon.h"
#include "Ray.h"
#include "Scene.h"

using namespace photon;

Photon::Photon(void)
{
}


Photon::~Photon(void)
{
}

Progress Photon::tick(double dist, Scene* scene){
	Ray r(position, destination);
	if((position-destination).mag() < dist) {
		return HIT;
	} else{
		HitRecord hr = scene->getHitRecord(r, false);
		if(hr.hit && hr.distance < dist){
			return DESTROY;
		} else {
			position = r.pointAt(dist);
			return CONTINUE;
		}
		
	}
}
