/*
 * Color.hpp
 *
 *  Created on: 26 lis 2017
 *      Author: Teresa Jarlaczyk
 */

#ifndef ENGINE_COMMON_COLOR_HPP_
#define ENGINE_COMMON_COLOR_HPP_

#include <iostream>

class Color {
public:
	Color();
	Color(int r, int g, int b, int a);
	Color(float r, float g, float b, float a);
	Color(int hexValue);
	Color(const Color & c);
	virtual ~Color();

	void SetOpacity(int a);
	void SetOpacity(float a);
	void SetColor(int r, int g, int b, int a);
	void SetColor(float r, float g, float b, float a);
	void Print();

	float r,g,b,a;
};

#endif /* ENGINE_COMMON_COLOR_HPP_ */
