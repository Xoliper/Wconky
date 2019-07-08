/*
 * ram.cpp
 *
 *  Created on: 17.10.2016
 *      Author: Arkadiusz Jarlaczyk
 */

#include "ram.hpp"

Ram::Ram(double refreshTime) {
	this->refreshTime = refreshTime;
	moduleType = "ram";
	entry = "{ram_value}";
	currentTime = 0.0;
	lastTime = glfwGetTime();
	load = 0.0f;
	std::cout<<"Ram Module: Loaded"<<std::endl;
}

Ram::~Ram() {
	std::cout<<"RAM module unloaded"<<std::endl;
}

void Ram::Update(){

	currentTime = glfwGetTime();
	if(currentTime - lastTime >= 0.2){
		GlobalMemoryStatus(&ms);
		load = ms.dwMemoryLoad;
		load /= 100;
		lastTime = currentTime;
	}
}

float Ram::GetValue(){
	return load;
}
