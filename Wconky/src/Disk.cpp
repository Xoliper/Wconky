/*
 * Disk.cpp
 *
 *  Created on: 4 gru 2017
 *      Author: Teresa Jarlaczyk
 */

#include "Disk.hpp"

//---------------------------------------------------
//DiskRow
//---------------------------------------------------

DiskRow::DiskRow(Disk * disk, World * world, std::string path, int x, int y, int width, Font * fnt, Color color, int borderHeight, int borderThickness, Color borderColor, Color fillColor){
	this->disk = disk;
	this->width = width;
	this->borderHeight = borderHeight;
	this->borderThickness = borderThickness;

	std::cout<<"DR: "<<this->borderHeight<<"  "<<this->borderThickness<<std::endl;


	dd.diskLetter = path;
	used = 0.0f;

	diskName = new Text(world, fnt, dd.diskLetter, x, y+borderHeight+10, 0);
	diskName->Colorize(color);
	val = new Text(world, fnt, "0.0%", x+width, y+borderHeight+10, 2);
	val->Colorize(color);

	outer = new Primitive(world, WorldRect(x, y,0,0), width, borderHeight, borderColor, 0);
	fill = new Primitive(world, WorldRect(x+borderThickness, y+borderThickness,0,0), width-2*borderThickness, borderHeight-2*borderThickness, fillColor, 0);

}

DiskRow::~DiskRow(){
	delete diskName;
	delete val;
	delete outer;
	delete fill;
}

void DiskRow::Update(){
	//GetDiskFreeSpaceEx(dd.diskLetter.c_str(), &dd.availableSpace, &dd.totalSpace, &dd.usedSpace);
	disk->updateStats(dd);
	used = 1.0f-(float)dd.usedSpace.QuadPart/dd.totalSpace.QuadPart;

	fill->Resize(  used*(width-2*borderThickness), borderHeight-2*borderThickness);
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << used*100;
	std::string valueStr = ss.str();
	std::string percent = "{value}%";
	percent.replace(0, 7, valueStr);
	replace( percent.begin(), percent.end(), ',', '.' );
	val->SetText(percent);
}

void DiskRow::Render(){
	diskName->Render();
	val->Render();
	outer->Render();
	fill->Render();
}


//---------------------------------------------------
//Disk
//---------------------------------------------------


Disk::Disk(double refreshTime) {
	//moduleType = "disk";
	//entry = "{disk}";
	this->refreshTime = refreshTime;
}

Disk::~Disk() {
}

bool Disk::checkDir(std::string path){
	DWORD dwAttr = GetFileAttributes(path.c_str());
	if(dwAttr != 0xffffffff && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)) return true;
	return false;
}

void Disk::updateStats(DiskData & dd){
	GetDiskFreeSpaceEx(dd.diskLetter.c_str(), &dd.availableSpace, &dd.totalSpace, &dd.usedSpace);
}

//---------------------------------------------------
//DiskHandler
//---------------------------------------------------

DiskHandler::DiskHandler(World * world, Disk * update, std::string msg, Theme * theme, int x, int y, int x_inc, int y_inc, int width){
	this->world = world;
	this->update = update;
	this->msg = msg;
	this->theme = theme;
	amount = 0;
	this->x = x;
	this->y = y;
	this->x_inc = x_inc;
	this->y_inc = y_inc;
	this->width = width;
	excluding = false;

	//Check mode
	size_t ifExcluding = msg.find("{exclude}");
	if(ifExcluding != std::string::npos){
		msg.erase(ifExcluding, 9);
		excluding = true;

		size_t s = 0;
		size_t e = 0;

		//get excluded
		while(s != std::string::npos and e != std::string::npos){
			s = msg.find('{',s);
			if(s != std::string::npos){
				e = msg.find('}',s);
				if(e != std::string::npos){
					std::string letter = msg.substr(s+1, e-s-1);
					ltr.push_back(letter[0]);
				}
				s = e;
			}
		}
	}
	std::cout<<"Disk Mode: "<<excluding<<std::endl;
	std::cout<<"LTR: "<<ltr.size()<<std::endl;
	for(int i = 0 ; i<ltr.size(); i++){
		std::cout<<ltr[i]<<std::endl;
	}

	Update();
}

DiskHandler::~DiskHandler(){
	for(std::list<DiskRow *>::iterator it = diskRows.begin(); it!=diskRows.end(); it++){
		delete (*it);
	}
}

void DiskHandler::Update(){

	//Create disk rows
	int x_temp = x;
	int y_temp = y;
	int testAmount = 0;



	if(msg.size() == 0 or msg == "0"){
		for(int i = 67; i <= 90; i++){ //A-Z
			char letter = static_cast<char>(i);
			std::string dir;
			dir.push_back(letter);
			dir += ":/"; //C:/

			if(update->checkDir(dir) == true){
				testAmount += 1;
			}
		}
		if(testAmount != amount){
			std::cout<<"USUWANIE!!"<<std::endl;

			for(std::list<DiskRow *>::iterator it = diskRows.begin(); it!=diskRows.end(); it++){
				delete (*it);
			}
			diskRows.clear();
			amount = 0;


			for(int i = 67; i <= 90; i++){ //A-Z
				char letter = static_cast<char>(i);
				std::string dir;
				dir.push_back(letter);
				dir += ":/"; //C:/
				if(update->checkDir(dir) == true){
					diskRows.push_back(new DiskRow(update, world, dir, x_temp, y_temp, width, theme->fnt, theme->color, theme->diskBorderHeight, theme->diskBorderThickness, theme->diskBorderCol, theme->diskFillCol));
					x_temp += x_inc;
					y_temp += y_inc;
					amount += 1;
				}
			}
		}

	} else {


		if(!excluding){

			//Check if amount change
			size_t s = 0;
			size_t e = 0;
			while(s != std::string::npos and e != std::string::npos){
				s = msg.find('{',s);
				if(s != std::string::npos){
					e = msg.find('}',s);
					if(e != std::string::npos){
						std::string letter = msg.substr(s+1, e-s-1);

						letter += ":/";
						if(update->checkDir(letter) == true){
							testAmount += 1;
						}
					}
					s = e;
				}
			}


			if(testAmount != amount){
				for(std::list<DiskRow *>::iterator it = diskRows.begin(); it!=diskRows.end(); it++){
					delete (*it);
				}
				diskRows.clear();
				amount = 0;

				s = 0;
				e = 0;

				while(s != std::string::npos and e != std::string::npos){
					s = msg.find('{',s);
					if(s != std::string::npos){
						e = msg.find('}',s);
						if(e != std::string::npos){
							std::string letter = msg.substr(s+1, e-s-1);
							letter += ":/";
							if(update->checkDir(letter) == true){
								//	DiskRow(World * world, std::string path, int x, int y, int width, Font * fnt, Color color);
								diskRows.push_back(new DiskRow(update, world, letter, x_temp, y_temp, width, theme->fnt, theme->color, theme->diskBorderHeight, theme->diskBorderThickness, theme->diskBorderCol, theme->diskFillCol));
								x_temp += x_inc;
								y_temp += y_inc;
								amount += 1;
							}
						}
						s = e;
					}
				}
			}

		} else {

			//Check if amount change
			size_t s = 0;
			size_t e = 0;
			for(int i = 67; i <= 90; i++){
				char letter = static_cast<char>(i);
				bool skip = false;
				for(int j = 0; j<ltr.size(); j++){
					if(letter == ltr[j]){
						skip = true;
						break;
					}
				}
				if(skip == true) continue;

				std::string dir;
				dir.push_back(letter);
				dir += ":/"; //C:/

				if(update->checkDir(dir) == true){
					testAmount += 1;
				}

			}


			if(testAmount != amount){
				for(std::list<DiskRow *>::iterator it = diskRows.begin(); it!=diskRows.end(); it++){
					delete (*it);
				}
				diskRows.clear();
				amount = 0;

				for(int i = 67; i <= 90; i++){ //A-Z

					char letter = static_cast<char>(i);
					bool skip = false;
					for(int j = 0; j<ltr.size(); j++){
						if(letter == ltr[j]){
							skip = true;
							break;
						}
					}
					if(skip == true) continue;

					std::string dir;
					dir.push_back(letter);
					dir += ":/"; //C:/
					if(update->checkDir(dir) == true){
						diskRows.push_back(new DiskRow(update, world, dir, x_temp, y_temp, width, theme->fnt, theme->color, theme->diskBorderHeight, theme->diskBorderThickness, theme->diskBorderCol, theme->diskFillCol));
						x_temp += x_inc;
						y_temp += y_inc;
						amount += 1;
					}
				}
			}
		}



	}


	for(std::list<DiskRow *>::iterator it = diskRows.begin(); it!=diskRows.end(); it++){
		(*it)->Update();
	}
}

void DiskHandler::Render(){
	for(std::list<DiskRow *>::iterator it = diskRows.begin(); it!=diskRows.end(); it++){
		(*it)->Render();
	}
}


