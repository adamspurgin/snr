// SeniorProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <windows.data.json.h>
#include <fstream>
#include <string>
#include <comutil.h>
#include <comdef.h>

#include "Scene.h"
#include "Sphere.h"
#include "Utils.h"
#include "Material.h"
#include "Motion.h"
#define interp(a, b, c)  (( (b) * (c) + (a) * ( 1 - (c))))


using namespace std;

void wtfIsWrong(){
	Vec3d a(1,2,3);
	Vec3d b(1,1,1);
	Vec3d c = (a - b).normal();
	c.toStream(std::cout);
}

void fromFileTest(){
	ifstream myfile;
	myfile.open("animtest.json");
	/*while(myfile.good()){
		char c = myfile.get();
		cout << c;
	}*/
	//myfile.seekg(0);
/*	istream* stripped = util::stripSpaces(myfile);

	while(stripped->good()){
		char c = stripped->get();
		cout << c;
	}*/
	Scene s(myfile);
	for(double i=0; i<100; i+= 1){

		_bstr_t a = L"fromFile";
		_bstr_t b = (double)i;
		_bstr_t suffix = ".bmp";
		Bitmap *img = s.trace(0, i/10.0);
		util::saveBitmap(a+b+suffix, img);

	}
}

void stripTest(){
	stringstream ss;
	ss << "{    type: SCN,    width: 2048,    height: 1536,    cameras: [{        type: camera,        loc: [0, 0, 0],        orientation: [-1.5708, -2.35278e-009, 0],        width: 36,        length: 36    }],    lights: [],    geometry: [{        type: sphere,        loc: [1005.2, 208.607, -364.686],        radius: 100,        material: [0, 0, 0],        motion: []    }, {        type: sphere,        loc: [868.415, -121.391, -316.64],        radius: 100,        material: [0, 0, 0],        motion: []    }, {        type: sphere,        loc: [423.263, 16.3085, 83.4392],        radius: 100,        material: [0, 0, 0],        motion: []    }]}";
	istream *ss2;
	ss2 = util::stripSpaces(ss);

	char test[1000];
	ss2->getline(test, 1000);
	printf("%s\n", test);
}

void motionTest(){
	cout << interp(10.0, 15.0, 0.5);
	Motion m;
	m.keyframes.push_back(KeyFrame(Vec3d(5,0,0), 0));
	m.keyframes.push_back(KeyFrame(Vec3d(10,20,15), 10));
	m.keyframes.push_back(KeyFrame(Vec3d(0,5,0), 5));
	for(int i=0; i<11; i++){
		m.atTick(i).toCSV(std::cout);
		std::cout << "\n";
	}
}


void renderTest(){
	CoInitialize(NULL);
	stringstream ss2;
	ss2 << "{type:SCN,width:2048,height:1536,cameras:[{type:camera,loc:[0,0,0],orientation:[-1.5708,-2.35278e-009,0],width:36,length:36}],lights:[],geometry:[{type:sphere,loc:[1005.2,208.607,-364.686],radius:100,material:[1,0,0],motion:[]},{type:sphere,loc:[868.415,-121.391,-316.64],radius:100,material:[0,1,0],motion:[]},{type:sphere,loc:[423.263,16.3085,83.4392],radius:100,material:[0,0,1],motion:[]}]}";
	Scene scene = Scene(ss2);
	Bitmap* b = scene.trace(0, 0);
	util::saveBitmap(L"testBitmap.bmp", b);
	CoUninitialize();
}

void geomTest(){
	stringstream ss;
	ss << "{type:sphere,loc:[1,2,3],radius:1337,material:[6,7,8]}";
	Geometry::fromStream(ss);
	cout << "\n";
	cout << ss.str();

	stringstream ss2;
	ss2 << "{type:SCN,width:2048,height:1536,cameras:[{type:camera,loc:[0,0,0],orientation:[-1.5708,-2.35278e-009,0],width:36,length:36}],lights:[],geometry:[{type:sphere,loc:[1005.2,208.607,-364.686],radius:100,material:[0,0,0]},{type:sphere,loc:[868.415,-121.391,-316.64],radius:100,material:[0,0,0]},{type:sphere,loc:[423.263,16.3085,83.4392],radius:100,material:[0,0,0]}]}";
	Scene scene = Scene(ss2);
	scene.toStream(cout);
}

void streamTests(){
	stringstream ss;
	ss << "[1,2,3]";
	Material m(1,2,3);
	Material m2(ss);
	m.toStream(cout);
	cout << "\n";
	m2.toStream(cout);
	cout << "\n";

	stringstream ss2;
	ss2 << "{type:sphere,loc:[1,2,3],radius:1337,material:[6,7,8]}";
	Sphere s;
	s.center = Vec3d(1,2,3);
	s.radius = 1337;
	s.material = Material(6,7,8);
	s.toStream(cout);
	cout << "\n";
	Sphere s2(ss2);
	s2.toStream(cout);
	cout << "\n";
}

void readTest(){
	cout << "starting read test\n";
	Scene *s = util::getFromXml("anim.xml");
	s->toStream(std::cout);
	//Bitmap* b = s->trace(0);
	cout << "\nending read test\n";
}

void matchTest(){
	stringstream ss;
	ss << "this is a test stream";
	util::match(ss, "this");
	util::match(ss, " is");
	util::match(ss, " a test");
	util::match(ss, " stream");
}


int _tmain(int argc, _TCHAR* argv[])
{
	//renderTest();
	//motionTest();
	
	CoInitialize(NULL);
	//stripTest();
	//renderTest();
	//readTest();
	fromFileTest();
	//wtfIsWrong();
	//printf("start using msxml6\n");

/*
	std::cout << "test\n";
	Scene s;
//	s.trace(0);

	s.addElement(new Light());
	s.addElement(new Camera());
	s.addElement(new Sphere());
	s.addElement(new Sphere());
	s.toStream(std::cout);
	matchTest();
	readTest();
	*/
	CoUninitialize();
	return 0;

//	streamTests();
//	geomTest();
}

