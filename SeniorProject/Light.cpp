#include "stdafx.h"
#include "Light.h"
#include "Utils.h"


Light::Light(void)
{
}

Light::Light(std::istream &in){
	util::match(in, "{type:light,loc:");
	position = Vec3d(in);
	util::match(in, "}");
}


Light::~Light(void)
{
}

void Light::toStream(std::ostream &str){
	str << "{type:light,loc:";
	position.toStream(str);
	str << "}";
}
