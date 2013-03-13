#pragma once

#include<iostream>

class SceneElement
{
public:
	SceneElement(void);
	~SceneElement(void);
	virtual void toStream(std::ostream&);
};

