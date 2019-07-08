/*
 * ResLoader.hpp
 *
 *  Created on: 20 kwi 2018
 *      Author: Xoliper
 */

#ifndef RESHOLDER_HPP_
#define RESHOLDER_HPP_

#include <windows.h>
#include <map>
#include <string>
#include "Resources.hpp"



class ResHolder {
public:
	ResHolder(HWND hwnd, HINSTANCE hInstance);
	virtual ~ResHolder();

	HWND hwnd;
	HINSTANCE hInst;

	std::map<int, HBITMAP> resCnt;
	std::map<int, HBRUSH> brushCnt;
	std::map<int, HFONT> fontCnt;

};

#endif /* RESHOLDER_HPP_ */
