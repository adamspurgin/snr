#pragma once
#include "Vec3d.h"

class KeyFrame
{
public:
	KeyFrame(void);
	KeyFrame(KeyFrame, double);
	KeyFrame(Vec3d, double);
	KeyFrame(std::istream&);
	~KeyFrame(void);
	static bool compare(KeyFrame, KeyFrame);
	void toStream(std::ostream&);
	void toCSV(std::ostream&);
	Vec3d position;
	double tick;

};

