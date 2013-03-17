#include "stdafx.h"
#include "KeyFrame.h"


KeyFrame::KeyFrame(void) {
	tick = 0;
}

KeyFrame::KeyFrame(KeyFrame f, long tick) {
	this->tick = tick;
	position = f.position;
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
