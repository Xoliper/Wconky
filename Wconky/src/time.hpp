#ifndef TIME_HPP_
#define TIME_HPP_

#include <string>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>
#include <regex>
#include "virtualclasses.hpp"
#include "Lang.hpp"

class TimeCnt : public Module {
public:
	TimeCnt(double refreshTime, Lang lang);
	virtual ~TimeCnt();

	void Update();
	float GetValue();

	double lastTime, currentTime;
	Lang lang;
	//double refreshTime;

	_SYSTEMTIME systemTime, backup;


};

#endif /* TIME_HPP_ */
