/*
 * ram.cpp
 *
 *  Created on: 17.10.2016
 *      Author: Arkadiusz Jarlaczyk
 */

#include "textcnt.hpp"

TextCnt::TextCnt(World * world, Module * updater, std::string msg, int xstart, int ystart, int flags, Font * fnt, Color textColor){
	this->world = world;
	this->updater = updater;
	this->fnt = fnt;
	msgTemplate = msg;
	msgToRender = msgTemplate;
	x = xstart; y = ystart;
	this->textColor = textColor;

	text = new Text(world, fnt, msgToRender, x, y, flags);
	text->Colorize(textColor);

	lastTime = glfwGetTime();
	currentTime = 0.0;

}

TextCnt::~TextCnt(){
	std::cout<<"TextCnt removed"<<std::endl;
	delete text;
}

void TextCnt::Update(){

	float value = 0;
	if(updater != nullptr){


		//"FPS" handling
		float rTime = (float)(1000/updater->refreshTime)/1000;  //0.1  -> 100
		currentTime = glfwGetTime();						   //0.33 -> 30
															   //1.0  -> 10
		if(currentTime - lastTime < rTime){                    //10   ->   1
			//return;
		}
		lastTime = currentTime;


		value = updater->GetValue()*100;
		msgToRender = msgTemplate;

		if(updater->moduleType == "cpu"){

			std::stringstream ss;
			ss << std::fixed << std::setprecision(2) << value;
			std::string valueStr = ss.str();

			size_t start_pos = msgToRender.find(updater->entry);
			if(start_pos != std::string::npos){
				msgToRender.replace(start_pos, updater->entry.length(), valueStr);
			}
		} else if(updater->moduleType == "ram"){
			std::stringstream ss;
			ss << std::fixed << std::setprecision(2) << value;
			std::string valueStr = ss.str();

			size_t start_pos = msgToRender.find(updater->entry);
			if(start_pos != std::string::npos){
				msgToRender.replace(start_pos, updater->entry.length(), valueStr);
			}
		} else if(updater->moduleType == "time"){
			PrepareTimeString();
		}
	}

	if(msgToRender != msgTemplate)
		text->SetText(msgToRender);

}

void TextCnt::Render(){
	text->Render();
}

void TextCnt::PrepareTimeString(){

	TimeCnt * timeTemp = static_cast<TimeCnt*>(updater);
	size_t f = -1;

	//Days
	f = msgToRender.find("{time_day_str}");
	if(timeTemp->lang.daysInit == true and f != std::string::npos){
		if(f != std::string::npos){
			int dayInt = timeTemp->systemTime.wDayOfWeek-1; //!
			if(dayInt < 0 or dayInt>7) dayInt = 6;
			std::string toReplace = timeTemp->lang.days[dayInt];
			msgToRender.replace(f, 14, toReplace);
		}
	} else {
		f = msgToRender.find("{time_day}");
		if(f != std::string::npos){
			int temp = (timeTemp->systemTime.wDay);
			std::string toReplace(std::to_string(temp));
			if(temp < 10){
				toReplace.insert(0, "0");
			}
			msgToRender.replace(f, 10, toReplace);
		}
	}

	//Months
	f = msgToRender.find("{time_month_str}");
	if(timeTemp->lang.monthsInit == true and f != std::string::npos){
		if(f != std::string::npos){
			int temp = (timeTemp->systemTime.wMonth);
			if(temp < 0 or temp > 12) temp = 0;
			std::string toReplace = timeTemp->lang.months[temp-1]; //!
			msgToRender.replace(f, 16, toReplace);
		}
	} else {
		f = msgToRender.find("{time_month}");
		if(f != std::string::npos){
			int temp = (timeTemp->systemTime.wMonth);
			std::string toReplace(std::to_string(temp));
			if(temp < 10){
				toReplace.insert(0, "0");
			}
			msgToRender.replace(f, 12, toReplace);
		}
	}


	//--------------------------------------------------------------------------------------------------------
	//Normal numbers
	//--------------------------------------------------------------------------------------------------------
	f = msgToRender.find("{time_year}");
	if(f != std::string::npos){
		int temp = (timeTemp->systemTime.wYear);
		std::string toReplace(std::to_string(temp));
		if(temp < 10){
			toReplace.insert(0, "0");
		}
		msgToRender.replace(f, 11, toReplace);
	}
	f = msgToRender.find("{time_hour}");
	if(f != std::string::npos){
		int temp = (timeTemp->systemTime.wHour);
		std::string toReplace(std::to_string(temp));
		if(temp < 10){
			toReplace.insert(0, "0");
		}
		msgToRender.replace(f, 11, toReplace);
	}
	f = msgToRender.find("{time_minute}");
	if(f != std::string::npos){
		int temp = (timeTemp->systemTime.wMinute);
		std::string toReplace(std::to_string(temp));
		if(temp < 10){
			toReplace.insert(0, "0");
		}
		msgToRender.replace(f, 13, toReplace);
	}

}

