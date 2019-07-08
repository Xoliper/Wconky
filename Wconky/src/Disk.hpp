/*
 * Disk.hpp
 *
 *  Created on: 4 gru 2017
 *      Author: Teresa Jarlaczyk
 */

#ifndef DISK_HPP_
#define DISK_HPP_

#include "Engine/Common/Primitive.hpp"
#include "Engine/Common/Text.hpp"
#include "Engine/Common/Font.hpp"
#include "Engine/Common/Color.hpp"
#include "virtualclasses.hpp"
#include "Engine/Common/world.hpp"

#include <string>
#include <sstream>
#include <iomanip>
#include <list>

#include "Theme.hpp"
class Disk;

typedef struct {
	std::string diskLetter;
	_ULARGE_INTEGER availableSpace;
	_ULARGE_INTEGER usedSpace;
	_ULARGE_INTEGER totalSpace;
} DiskData;

class DiskRow : public Drawable {
public:
	DiskRow(Disk * disk, World * world, std::string path, int x, int y, int width, Font * fnt, Color color,  int borderHeight, int borderThickness, Color borderColor, Color fillColor);
	~DiskRow();

	void Update();
	void Render();

	Disk * disk;
	DiskData dd;
	float used;
	int width;
	int borderHeight;
	int borderThickness;

	Text * diskName;
	Text * val;
	Primitive * outer, * fill;

};

class DiskHandler : public Drawable, public ToUpdate {

public:
	DiskHandler(World * world, Disk * update, std::string msg, Theme * theme, int x, int y, int x_inc, int y_inc, int width);
	~DiskHandler();

	void Update();
	void Render();

private:
	Disk * update;
	Theme * theme;
	World * world;

	std::vector<char> ltr;
	std::list<DiskRow *> diskRows;
	int amount;
	std::string msg;

	int x, y, x_inc, y_inc;
	int width;
	bool excluding;
};


class Disk {

	friend class DiskHandler;
	friend class DiskRow;

public:
	Disk(double refreshTime);
	virtual ~Disk();

private:
	bool checkDir(std::string path);
	void updateStats(DiskData & dd);
	float refreshTime;

};

#endif /* DISK_HPP_ */
