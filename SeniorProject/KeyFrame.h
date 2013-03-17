#pragma once
#include "Vec3d.h"

class KeyFrame
{
public:
	KeyFrame(void);
	KeyFrame(KeyFrame, long);
	~KeyFrame(void);
	static bool compare(KeyFrame, KeyFrame);
	void toStream(std::ostream&);
	Vec3d position;
	long tick;

};

