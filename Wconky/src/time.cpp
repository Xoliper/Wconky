/*
 * time.cpp
 *
 *  Created on: 29 lis 2017
 *      Author: Teresa Jarlaczyk
 */

#include "time.hpp"

TimeCnt::TimeCnt(double refreshTime, Lang lang) {
	this->refreshTime = refreshTime;
	this->lang = lang;
	moduleType = "time";
	entry = "{time_day}{time_month}{time_year}{time_hour}{time_minute}{time_day_str}{time_month_str}";
	lastTime = glfwGetTime();
	currentTime = lastTime;
	std::cout<<"Time Module: Loaded"<<std::endl;
}

TimeCnt::~TimeCnt() {
	std::cout<<"TIME module unloaded"<<std::endl;
}

void TimeCnt::Update(){
	currentTime = glfwGetTime();
	if(currentTime - lastTime >= 1.0){

		//Do update
		GetLocalTime(&systemTime);
		lastTime = currentTime;
	}
}

float TimeCnt::GetValue(){
	return 0.0f;
}
