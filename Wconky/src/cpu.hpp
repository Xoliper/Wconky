/*
 * cpu.hpp
 *
 *  Created on: 17.10.2016
 *      Author: Arkadiusz Jarlaczyk
 */

#ifndef CPU_HPP_
#define CPU_HPP_

#include <windows.h>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include "virtualclasses.hpp"

class Cpu : public Module {
public:
	Cpu(double refreshTime);
	virtual ~Cpu();

	void Update();
	float GetValue();

private:
	float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
	unsigned long long FileTimeToInt64(const FILETIME & ft);
	float GetCPULoad();

	typedef BOOL (WINAPI *MYFUNC2) ( LPFILETIME , LPFILETIME , LPFILETIME);
	MYFUNC2 GetSystemTimesD;

	float load;
	double lastTime, currentTime;
	//double refreshTime;

};

#endif /* CPU_HPP_ */
