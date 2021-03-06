#pragma once
#include <iostream>
#include "Scene.h"

#ifndef DEBUGMODE
#define DEBUGMODE 0
#endif

#define DEBUG() if(DEBUGMODE) printf("%s: %d\n", __FILE__, __LINE__);

class Utils
{
public:
	Utils(void);
	~Utils(void);
};

namespace util{
	void saveBitmap(BSTR, Bitmap*);
	int GetEncoderClsid(const WCHAR*, CLSID*);
	void match(std::istream&, const char*);
	void eat(std::istream&);
	void debug(char*);
	Scene* getFromXml(const char*);
	bool quadRoots(double, double, double, double*, double*);
	std::istream* stripSpaces(std::istream&);
}

