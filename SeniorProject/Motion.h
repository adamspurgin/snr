#pragma once

#include "KeyFrame.h"
#include <vector>

class Motion
{
private:
	bool isSorted;
	void sort();
public:
	Motion(std::istream&);
	Motion(void);
	~Motion(void);
	std::vector<KeyFrame> keyframes;
	void toStream(std::ostream&);
	KeyFrame atTick(double);
	bool isEmpty();
};

