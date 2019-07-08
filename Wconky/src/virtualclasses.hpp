/*
 * module.hpp
 *
 *  Created on: 28 lis 2017
 *      Author: Teresa Jarlaczyk
 */

#ifndef VIRTUALCLASSES_HPP_
#define VIRTUALCLASSES_HPP_

#include <string>

class Module {
public:
	Module(){};
	virtual ~Module(){};
	virtual void Update() = 0;
	virtual float GetValue() = 0;
	std::string moduleType;
	std::string entry;
	double refreshTime = 0.0;
};

class ToUpdate {
public:
	ToUpdate(){};
	virtual ~ToUpdate(){};
	virtual void Update() = 0;

};

#endif /* VIRTUALCLASSES_HPP_ */
