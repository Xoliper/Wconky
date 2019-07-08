/*
 * Lang.hpp
 *
 *  Created on: 3 gru 2017
 *      Author: Teresa Jarlaczyk
 */

#ifndef LANG_HPP_
#define LANG_HPP_

#include <string>

class Lang {
public:
	Lang();
	virtual ~Lang();

	std::string days[7];
	std::string months[12];
	bool daysInit, monthsInit;

};

#endif /* LANG_HPP_ */
