#pragma once
#include <iostream>

class Material
{
public:
	Material(void);
	Material(std::istream&);
	Material(double, double, double);
	~Material(void);
	void set(double, double, double);
	double getRed();
	double getGreen();
	double getBlue();
	double getHue();
	double getSat();
	double getVal();
	void toStream(std::ostream&);
private:
	double r, g, b;
};

