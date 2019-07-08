/*
 * Color.cpp
 *
 *  Created on: 26 lis 2017
 *      Author: Teresa Jarlaczyk
 */

#include "Color.hpp"

Color::Color() {
 r = g = b = a = 1.0f;
}

Color::Color(int r, int g, int b, int a){
	this->r = (float)r/255;
	this->g = (float)g/255;
	this->b = (float)b/255;
	this->a = (float)a/255;
}

Color::Color(float r, float g, float b, float a){
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color::Color(int hexValue){
	r = (float)((hexValue >> 16) & 0xFF)/255;  // Extract the RR byte
	g = (float)((hexValue >> 8) & 0xFF)/255 ;   // Extract the GG byte
	b = (float)((hexValue) & 0xFF)/255 ;        // Extract the BB byte
	a = 1.0;
}

Color::Color(const Color & c){
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
}

Color::~Color() {
}

void Color::SetOpacity(int a){
	this->a = (float)a/255;
}

void Color::SetOpacity(float a){
	this->a = a;
}

void Color::SetColor(int r, int g, int b, int a){
	this->r = (float)r/255;
	this->g = (float)g/255;
	this->b = (float)b/255;
	this->a = (float)a/255;
}

void Color::SetColor(float r, float g, float b, float a){
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void Color::Print(){
	std::cout<<"Color data: "<<r<<"  "<<g<<"   "<<b<<std::endl;
}
