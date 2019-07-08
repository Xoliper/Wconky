#ifndef RAM_HPP_
#define RAM_HPP_


#include <string>
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>
#include "virtualclasses.hpp"

class Ram : public Module {
public:
	Ram(double refreshTime);
	virtual ~Ram();

	void Update();
	float GetValue();

private:
	float load;
	double lastTime, currentTime;
	//double refreshTime;
	MEMORYSTATUS ms;

};

#endif /* RAM_HPP_ */
