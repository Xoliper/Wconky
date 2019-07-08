/*
 * Theme.hpp
 *
 *  Created on: 27 lis 2017
 *      Author: Teresa Jarlaczyk
 */

#ifndef THEME_HPP_
#define THEME_HPP_

#include "Engine/Common/Font.hpp"
#include "Engine/Common/Color.hpp"

class Theme {
public:
	Theme();
	~Theme();

	//font
	Font * fnt;
	Color color;

	//graphTheme
	Color graphBars;
	Color graphBase;
	int graphBarWidth;
	int graphBarSpacing;

	//diskTheme
	Color diskBorderCol;
	Color diskFillCol;
	int diskBorderThickness;
	int diskBorderHeight;

};

#endif /* THEME_HPP_ */
