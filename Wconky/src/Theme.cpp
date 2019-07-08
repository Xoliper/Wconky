/*
 * Theme.cpp
 *
 *  Created on: 27 lis 2017
 *      Author: Teresa Jarlaczyk
 */

#include "Theme.hpp"

Theme::Theme() {
	fnt = nullptr;
	graphBars = graphBase = diskBorderCol = diskFillCol = 0;
	graphBarWidth = 2;
	graphBarSpacing = 1;
	diskBorderThickness = 1;
	diskBorderHeight = 15;
}

Theme::~Theme() {
	std::cout<<"Theme deleted"<<std::endl;
	if(fnt != nullptr) delete fnt;
}

