#include "stdafx.h"
#include "Motion.h"
#include "Utils.h"
#include <algorithm>


#define doSort() if(!isSorted) {sort(); isSorted=true;}
#define interp(a, b, c)  (( (b) * (c) + (a) * ( 1 - (c))))

Motion::Motion(std::istream& str){

}

Motion::Motion(void)
{
	isSorted = false;
}


Motion::~Motion(void)
{
}

void Motion::sort(){
	std::sort(keyframes.begin, keyframes.end, KeyFrame::compare);
}

void Motion::toStream(std::ostream& str){
	str << "[";
	for(int i=0; i<keyframes.size(); i++){
		keyframes[i].toStream(str);
		if(i != keyframes.size()-1){
			str << ",";
		}
	}
	str << "]";
}

KeyFrame Motion::atTick(long tick){
	doSort();
	for(int i=0; i<keyframes.size(); i++){
		if(i == 0 && keyframes[i].tick >= tick){
			return KeyFrame(keyframes[i], tick);
		} else if (i == keyframes.size() - 1 && keyframes[i].tick <= tick){
			return KeyFrame(keyframes[i], tick);
		} else if(keyframes[i].tick <= tick && keyframes[i+1].tick >= tick) {
			long range = keyframes[i+1].tick - keyframes[i].tick;
			if(range == 0){
				return keyframes[i];
			}
			long progress = tick - keyframes[i].tick;
			double frac = progress/range;

			double x = interp(keyframes[i].position.x, keyframes[i+1].position.x, frac);
			double y = interp(keyframes[i].position.y, keyframes[i+1].position.y, frac);
			double z = interp(keyframes[i].position.z, keyframes[i+1].position.z, frac);
			Vec3d location(x, y, z);
			KeyFrame ret;
			ret.position = location;
			ret.tick = tick;
			return ret;
		}
	}
}

