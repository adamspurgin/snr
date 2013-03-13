#pragma once
#include <iostream>
#include "Scene.h"

class Utils
{
public:
	Utils(void);
	~Utils(void);
};

namespace util{
	void match(std::istream&, const char*);
	void eat(std::istream&);
	void debug(char*);
	Scene* getFromXml(const char*);
	bool quadRoots(double, double, double, double*, double*);
}

