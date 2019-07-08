/*
 * cpu.cpp
 *
 *  Created on: 17.10.2016
 *      Author: Arkadiusz Jarlaczyk
 */

#include "cpu.hpp"

Cpu::Cpu(double refreshTime) {
	this->refreshTime = refreshTime;
	load = 0.0f;
	moduleType = "cpu";
	entry = "{cpu_value}";
	//graph = new Graph(world, 1920-500, 90, 400, 75, theme->graphBars, theme->graphBase, theme->graphBarWidth);

	GetSystemTimesD = NULL;

	HINSTANCE hInst = LoadLibrary( "Kernel32.dll");
	if( hInst )
	{
		GetSystemTimesD = (MYFUNC2) GetProcAddress( hInst, "GetSystemTimes" );
		if( GetSystemTimesD ){
			FreeLibrary( hInst );
			std::cout<<"CPU Module: Loaded"<<std::endl;
		} else {
			FreeLibrary( hInst );
			std::cout<<"CPU Module: initialization failed"<<std::endl;
		}
	}

	std::cout<<"CPU: asdasd "<<this->refreshTime<<std::endl;

	currentTime = 0.0;
	lastTime = glfwGetTime();
}

Cpu::~Cpu() {
	std::cout<<"CPU module unloaded"<<std::endl;
}

void Cpu::Update(){
	currentTime = glfwGetTime();
	if(currentTime - lastTime >= 0.8){
		load = GetCPULoad();
		lastTime = currentTime;
	//	std::cout<<load<<std::endl;
	}
}

float Cpu::GetValue(){
	return load;
}

float Cpu::CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
   static unsigned long long _previousTotalTicks = 0;
   static unsigned long long _previousIdleTicks = 0;

   unsigned long long totalTicksSinceLastTime = totalTicks-_previousTotalTicks;
   unsigned long long idleTicksSinceLastTime  = idleTicks-_previousIdleTicks;

   float ret = 1.0f-((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime)/totalTicksSinceLastTime : 0);

   _previousTotalTicks = totalTicks;
   _previousIdleTicks  = idleTicks;
   return ret;
}

unsigned long long  Cpu::FileTimeToInt64(const FILETIME & ft) {
	return (((unsigned long long)(ft.dwHighDateTime))<<32)|((unsigned long long)ft.dwLowDateTime);
}

float  Cpu::GetCPULoad()
{
   FILETIME idleTime, kernelTime, userTime;
   return GetSystemTimesD(&idleTime, &kernelTime, &userTime) ? CalculateCPULoad(FileTimeToInt64(idleTime), FileTimeToInt64(kernelTime)+FileTimeToInt64(userTime)) : -1.0f;
}
