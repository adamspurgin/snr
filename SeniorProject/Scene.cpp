#include "stdafx.h"
#include "Scene.h"
#include "Utils.h"
#include <gdiplus.h>

using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")

Scene::Scene(void)
{
}

Scene::Scene(std::istream& in){
	util::match(in, "{type:SCN,width:");
	in >> width;
	util::match(in, ",height:");
	in >> height;
	util::match(in, ",cameras:[");
	while(in.get() == '{'){
		in.unget();
		this->addElement(new Camera(in));
		if(in.get() != ','){
			in.unget();
		}
	}
	in.unget();

	util::match(in, "],lights:[");
	while(in.get() == '{'){
		in.unget();
		this->addElement(new Light(in));
		if(in.get() != ','){
			in.unget();
		}
	}
	in.unget();

	util::match(in, "],geometry:[");
	while(in.get() == '{'){
		in.unget();
		this->addElement(Geometry::fromStream(in));
		if(in.get() != ','){
			in.unget();
		}
	}
	in.unget();
	util::match(in, "]}");
	//TODO:(the rest of this)
}

Scene::~Scene(void)
{
}

void Scene::toStream(std::ostream& str){
	str << "{type:SCN,width:" << width << ",height:"<<height<<",";
	str << "cameras:[";
	for(unsigned int i=0; i < cameras.size(); i++){
		cameras[i]->toStream(str);
		if(i != cameras.size() - 1){
			str << ",";
		}
	}
	str << "],lights:[";
	for(unsigned int i=0; i < lights.size(); i++){
		lights[i]->toStream(str);
		if(i != lights.size() - 1){
			str << ",";
		}
	}
	str << "],geometry:[";
	for(unsigned int i=0; i < geom.size(); i++){
		geom[i]->toStream(str);
		if(i != geom.size() - 1){
			str << ",";
		}
	}
	str << "]}";
}

void Scene::addElement(Geometry* g){
	geom.push_back(g);
}

void Scene::addElement(Light* l){
	lights.push_back(l);
}

void Scene::addElement(Camera* l){
	cameras.push_back(l);
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

Bitmap* Scene::trace(int cam){
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	CLSID   encoderClsid;
	GetEncoderClsid(L"image/bmp", &encoderClsid);
	Bitmap* b = new Bitmap(width, height);

	//trace loop
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			Ray r = getRay(i, j, cam);
			bool hit = false;
			for(int g=0; g<geom.size(); g++){
				hit |= (geom[g]->intersect(r) > 0);
			}
			if(hit){
				b->SetPixel(j, i, Gdiplus::Color(255, 255, 255));
			} else {
				b->SetPixel(j, i, Gdiplus::Color(0, 0, 0));
			}
		}
	}

	b->Save(L"test.bmp", &encoderClsid, NULL);
	
	return b;
}



Ray Scene::getRay(int row, int col, int cam){
	Camera c = *cameras[cam];
	Vec3d start = c.position;
	double tick = c.focalWidth / width;
	Vec3d end;
	end.x = c.focalLength;
	end.y = (-row + height/2) * tick;
	end.z = (-col + width/2) * tick;
	end = end + start;
	return Ray(start, end);
}