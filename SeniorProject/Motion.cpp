#include "stdafx.h"
#include "Motion.h"
#include "Utils.h"
#include <algorithm>



#define doSort() if(!isSorted) {sort(); isSorted=true;}
#define interp(a, b, c)  (( (b) * (c) + (a) * ( 1 - (c))))

Motion::Motion(std::istream& str){
	util::match(str, "[");
	while(str.peek() != ']'){
		keyframes.push_back(KeyFrame(str));
		if(str.peek() == ',') str.get();
	}
	util::match(str, "]");
}

Motion::Motion(void)
{
	isSorted = false;
}


Motion::~Motion(void)
{
}

bool compare(KeyFrame a, KeyFrame b){
	return a.tick < b.tick;
}

void Motion::sort(){
	std::sort(keyframes.begin(), keyframes.end(), compare);
}



void Motion::toStream(std::ostream& str){
	str << "[";
	for(unsigned int i=0; i < keyframes.size(); i++){
		keyframes[i].toStream(str);
		if(i != keyframes.size()-1){
			str << ",";
		}
	}
	str << "]";
}

KeyFrame Motion::atTick(double tick){
	doSort();
	for(unsigned int i=0; i<keyframes.size(); i++){
		if(i == 0 && keyframes[i].tick >= tick){
			return KeyFrame(keyframes[i], tick);
		} else if (i == keyframes.size() - 1 && keyframes[i].tick <= tick){
			return KeyFrame(keyframes[i], tick);
		} else if(keyframes[i].tick <= tick && keyframes[i+1].tick >= tick) {
			double range = keyframes[i+1].tick - keyframes[i].tick;
			if(range == 0){
				return keyframes[i];
			}
			double progress = tick - keyframes[i].tick;
			double frac = progress/range;

			double x = interp(keyframes[i].position.x, keyframes[i+1].position.x, frac);
			//std::cout << x << "\n";
			double y = interp(keyframes[i].position.y, keyframes[i+1].position.y, frac);
			//std::cout << y << "\n";
			double z = interp(keyframes[i].position.z, keyframes[i+1].position.z, frac);
			//std::cout << z << "\n";
			Vec3d location(x, y, z);
			KeyFrame ret;
			ret.position = location;
			ret.tick = tick;
			return ret;
		}
	}
	return KeyFrame(keyframes[keyframes.size() - 1], tick);
}


bool Motion::isEmpty(){
	return (keyframes.size() == 0);
}