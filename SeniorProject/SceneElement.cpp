#include "stdafx.h"
#include "SceneElement.h"


SceneElement::SceneElement(void){
}


SceneElement::~SceneElement(void){
}

void SceneElement::toStream(std::ostream& str){
	str << "testElement";
}