#include "stdafx.h"
#include "Camera.h"
#include "Utils.h"

Camera::Camera(std::istream &in){
	util::match(in, "{type:camera,loc:");
	position = Vec3d(in);
	util::match(in, ",orientation:");
	rotation = Vec3d(in);
	util::match(in, ",width:");
	in >> focalWidth;
	util::match(in, ",length:");
	in >> focalLength;
	util::match(in, "}");
}

Camera::Camera(void)
{
}


Camera::~Camera(void)
{
}

void Camera::toStream(std::ostream& strm){
	strm << "{type:camera,loc:";
	position.toStream(strm);
	strm << ",orientation:";
	rotation.toStream(strm);
	strm << ",width:" << focalWidth << ",length:" <<  focalLength << "}";
}