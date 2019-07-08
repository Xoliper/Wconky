/*
 * graph.hpp
 *
 *  Created on: 17.10.2016
 *      Author: Arkadiusz Jarlaczyk
 */

#ifndef TEXTCNT_HPP_
#define TEXTCNT_HPP_

#include "virtualclasses.hpp"
#include "Engine/Common/drawable.hpp"
#include "Engine/Common/world.hpp"
#include "Engine/Common/color.hpp"
#include "Engine/Common/font.hpp"
#include "Engine/Common/text.hpp"
#include "Engine/Common/Conversions.hpp"
#include <string>
#include <iomanip>
#include <sstream>
#include "time.hpp"
#include "Disk.hpp"

using namespace std;


class TextCnt : public Drawable, public ToUpdate {
public:
	TextCnt(World * world, Module * updater, std::string msg, int xstart, int ystart, int flags, Font * fnt, Color textColor);
	~TextCnt();

	void Update();
	void Render();

private:

	void PrepareTimeString();

	//Main stuff
	World * world;
	Module * updater;
	Font * fnt;
	Text * text;

	//Data holders
	Color textColor;
	int x, y;
	std::string msgTemplate;
	std::string msgToRender;
	double lastTime, currentTime;

};

#endif /* TEXTCNT_HPP_ */
