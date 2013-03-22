#include "stdafx.h"
#include "KeyFrame.h"
#include "Utils.h"


KeyFrame::KeyFrame(void) {
	tick = 0;
}

KeyFrame::KeyFrame(KeyFrame f, double tick) {
	this->tick = tick;
	position = f.position;
}

KeyFrame::KeyFrame(Vec3d p, double t) {
	tick = t;
	position = p;
}

KeyFrame::KeyFrame(std::istream& in){
	util::match(in, "{");
	in >> tick;
	util::match(in, ",");
	position =  Vec3d(in);
	util::match(in, "}");
}


KeyFrame::~KeyFrame(void)
{
}

static bool compare(KeyFrame a, KeyFrame b){
	return a.tick < b.tick;
}

void KeyFrame::toStream(std::ostream& str){
	str << "{" << tick << ",";
	position.toStream(str);
	str << "}";
}

void KeyFrame::toCSV(std::ostream& str){
	str << tick << ", " << position.x << ", " << position.y << ", " << position.z;
}
