/*
 * Settings.hpp
 *
 *  Created on: 8 sie 2018
 *      Author: Xoliper
 */

#ifndef SETTINGS_HPP_
#define SETTINGS_HPP_

#include <string>
#include <windows.h>
#include <iostream>
#include <list>
#include <tlhelp32.h>
#include "Resources.hpp"
#include "Libs/XmlParser.hpp"


#define TS(VAR) static_cast<TextSettings*>(VAR)
#define GS(VAR) static_cast<GraphSettings*>(VAR)
#define DS(VAR) static_cast<DiskSettings*>(VAR)

void GetAllWindowsFromProcessID(DWORD dwProcessID, std::vector <HWND> &vhWnds);

class EditorWindow;

typedef struct {
	int size;
	std::string color;
	int opacity;
	std::string path;
} FontTheme;

typedef struct {
	std::string graphBarsCol;
	std::string graphBaseCol;
	int opacity;
	int barWidth;
	int barSpacing;
} GraphTheme;

typedef struct {
	int borderThickness;
	std::string borderColor;
	std::string fillColor;
	int opacity;
	int borderHeight;
} DiskTheme;

typedef struct {
	bool font = 0;			//only -> text
	bool graph = 0;			//only -> graph
	bool disk = 0;			//font + disk -> disk
	FontTheme ft;
	GraphTheme gt;
	DiskTheme dt;
} ThemeInfo;

typedef struct {
	int monitor;
	int delay;
	bool backgroundAuto;
	int backgroundMode;
	std::string backgroundPath;
	int bgOpacity;
	bool cpu;
	int cpuFps;
	bool ram;
	int ramFps;
	bool disk;
	int diskFps;
	bool time;
	int timeFps;
	int extOn;
	int extHK;
} GeneralSettings;


typedef struct {
	std::string monday;
	std::string tuesday;
	std::string wednesday;
	std::string thursday;
	std::string friday;
	std::string saturday;
	std::string sunday;
	std::string jan;
	std::string feb;
	std::string mar;
	std::string apr;
	std::string may;
	std::string june;
	std::string july;
	std::string aug;
	std::string sept;
	std::string oct;
	std::string nov;
	std::string dec;
} LanguageSettings;


typedef struct {
	std::string itemName;	//add
	std::string text;
	int posx = 0;
	int posy = 0;
	int alignment = 0;
	std::string fontPath;
	std::string color;
	int fontSize = 0;
	int opacity = 255;
} TextSettings;

typedef struct {
	std::string itemName;
	std::string text;	//add
	int posx = 0;
	int posy = 0;
	int width = 0;
	int height = 0;
	std::string baseColor;
	std::string barColor;
	int barWidth = 1;
	int barSpace = 0;
	int opacity = 255;
} GraphSettings;

typedef struct {
	//object part
	std::string itemName;
	std::string text;
	int posx = 0;
	int posy = 0;
	int posx_inc = 0;
	int posy_inc = 0;
	int width = 0;
	//font part
	std::string fontPath;
	std::string color;
	int fontSize = 0;
	int opacity = 255;
	//disk part
	int borderThickness = 1;
	std::string borderColor;
	std::string fillColor;
	int diskOpacity = 255;
	int borderHeight = 0;
} DiskSettings;

class Settings {
public:
	Settings();
	virtual ~Settings();

	void SaveAsXmlConfig(std::string path);

	void LoadGSettings(XmlParser * parser);
	void LoadLSettings(XmlParser * parser);
	void LoadItems(XmlParser * parser, EditorWindow * ew);

	std::string ReadEditStr(int resID, HWND hDlg);
	int ReadInt(int resID, HWND hDlg);
	int ReadRatio(int resFirst, int resSec, HWND hDlg);

	void ReadGSettings(HWND hDlg);
	void ReadLSettings(HWND hDlg);


	void ReadTextSettings(int itemId, HWND hDlg); //TODO	//itemID -> where to save data?!
	void ReadGraphSettings(int itemId, HWND hDlg); //TODO
	void ReadDiskSettings(int itemId, HWND hDlg);


	std::list<void*> items;
	std::list<int> itemsTypeId;
	GeneralSettings gs;
	LanguageSettings ls;

	HWND hDlg;

private:
	void LoadTextSettings(TextSettings * ts,  Node * node, XmlParser * pr);
	void LoadGraphSettings(GraphSettings * gs, Node * node, XmlParser * pr);
	void LoadDiskSettings(DiskSettings * ds, Node * node, XmlParser * pr);
	Node * GetThemeNode(XmlParser * pr, int themeId);


};

#endif /* SETTINGS_HPP_ */
