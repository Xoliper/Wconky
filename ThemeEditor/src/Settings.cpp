/*
 * Settings.cpp
 *
 *  Created on: 8 sie 2018
 *      Author: Xoliper
 */

#include "Settings.hpp"
#include "EditorWindow.hpp"



void GetAllWindowsFromProcessID(DWORD dwProcessID, std::vector <HWND> &vhWnds)
{
    // find all hWnds (vhWnds) associated with a process id (dwProcessID)
    HWND hCurWnd = NULL;
    do
    {
        hCurWnd = FindWindowEx(NULL, hCurWnd, NULL, NULL);
        DWORD dwProcessID = 0;
        GetWindowThreadProcessId(hCurWnd, &dwProcessID);
        if (dwProcessID == dwProcessID)
        {
            vhWnds.push_back(hCurWnd);  // add the found hCurWnd to the vector
            wprintf(L"Found hWnd %d\n", hCurWnd);
        }
    }
    while (hCurWnd != NULL);
}



Settings::Settings() {
}

Settings::~Settings() {
}

void Settings::SaveAsXmlConfig(std::string path){

	std::vector<ThemeInfo> themes;
	std::string bgm;
	if(gs.backgroundMode == 0) bgm = "stretch";
	if(gs.backgroundMode == 1) bgm = "oryginal";
	if(gs.backgroundMode == 2) bgm = "fit-in";
	if(gs.backgroundMode == 3) bgm = "fit-out";


	XmlParser xml;
	xml.prepareRootNode();

	Node * n;
	n = xml.addNode("configurations", 	{""});
	n = xml.addNode("config", 			{"configurations"});
	xml.setAttribute(n, "id", "0");


	n = xml.addNode("settings", 		{"configurations", "config"});
	n = xml.addNode("delay", 			{"configurations", "config", "settings"});
	xml.setNodeValue(n, std::to_string(gs.delay));
	n = xml.addNode("monitor", 			{"configurations", "config", "settings"});
	xml.setNodeValue(n, std::to_string(gs.monitor));
	n = xml.addNode("background",		{"configurations", "config", "settings"});
	if(gs.backgroundPath.size() > 0) xml.setNodeValue(n, gs.backgroundPath);
	if(gs.backgroundAuto == false) xml.setNodeValue(n, "");
	xml.setAttribute(n, "mode", bgm);
	xml.setAttribute(n, "opacity", std::to_string(gs.bgOpacity));


	n = xml.addNode("modules",			{"configurations", "config"});
	n = xml.addNode("cpu",				{"configurations", "config", "modules"});
	xml.setNodeValue(n, std::to_string(gs.cpu));
	xml.setAttribute(n, "fps", std::to_string(gs.cpuFps));
	n = xml.addNode("ram",				{"configurations", "config", "modules"});
	xml.setNodeValue(n, std::to_string(gs.ram));
	xml.setAttribute(n, "fps", std::to_string(gs.ramFps));
	n = xml.addNode("disk",				{"configurations", "config", "modules"});
	xml.setNodeValue(n, std::to_string(gs.disk));
	xml.setAttribute(n, "fps", std::to_string(gs.diskFps));
	n = xml.addNode("time",				{"configurations", "config", "modules"});
	xml.setNodeValue(n, std::to_string(gs.time));
	xml.setAttribute(n, "fps", std::to_string(gs.timeFps));


	n = xml.addNode("lang", 			{"configurations", "config"});
	n = xml.addNode("days",				{"configurations", "config", "lang"});
	xml.setAttribute(n, "monday", ls.monday);
	xml.setAttribute(n, "tuesday", ls.tuesday);
	xml.setAttribute(n, "wednesday", ls.wednesday);
	xml.setAttribute(n, "thursday", ls.thursday);
	xml.setAttribute(n, "friday", ls.friday);
	xml.setAttribute(n, "saturday", ls.saturday);
	xml.setAttribute(n, "sunday", ls.sunday);
	n = xml.addNode("months",			{"configurations", "config", "lang"});
	xml.setAttribute(n, "jan", ls.jan);
	xml.setAttribute(n, "feb", ls.feb);
	xml.setAttribute(n, "mar", ls.mar);
	xml.setAttribute(n, "apr", ls.apr);
	xml.setAttribute(n, "may", ls.may);
	xml.setAttribute(n, "june", ls.june);
	xml.setAttribute(n, "july", ls.july);
	xml.setAttribute(n, "aug", ls.aug);
	xml.setAttribute(n, "sept", ls.sept);
	xml.setAttribute(n, "oct", ls.oct);
	xml.setAttribute(n, "nov", ls.nov);
	xml.setAttribute(n, "dec", ls.dec);

	if(gs.extOn == true){
		n = xml.addNode("Extensions", 	{"configurations", "config"});
		n = xml.addNode("ManageIcons", 	{"configurations", "config", "Extensions"});
		xml.setAttribute(n, "hotkey", std::to_string(gs.extHK));
	}



	n = xml.addNode("visualObjects",			{"configurations", "config"});
	auto typeId = itemsTypeId.begin();
	for(std::list<void*>::iterator it = items.begin(); it!=items.end(); it++){

		if((*typeId) == 2){
			TextSettings * ts = TS((*it));

			n = xml.addNode("object",			{"configurations", "config", "visualObjects"});
			xml.setNodeValue(n, ts->text);
			xml.setAttribute(n, "type", "text");
			xml.setAttribute(n, "itemName", ts->itemName);
			xml.setAttribute(n, "x", std::to_string(ts->posx));
			xml.setAttribute(n, "y", std::to_string(ts->posy));
			xml.setAttribute(n, "align", std::to_string(ts->alignment));

			//Finding theme
			int themeId = -1;
			for(size_t i = 0; i<themes.size(); i++){
				if(ts->color == themes[i].ft.color and ts->fontPath == themes[i].ft.path and ts->fontSize == themes[i].ft.size and ts->opacity == themes[i].ft.opacity){
					//Found!
					xml.setAttribute(n, "useTheme", std::to_string(i));
					themeId = i;
				}
			}
			//Create new theme
			if(themeId == -1){
				ThemeInfo ti;
				FontTheme ft;
				ft.color = ts->color;
				ft.opacity = ts->opacity;
				ft.path = ts->fontPath;
				ft.size = ts->fontSize;
				ti.ft = ft;
				ti.font = true;
				themes.push_back(ti);
				xml.setAttribute(n, "useTheme", std::to_string(themes.size()-1));
			}


		} else if((*typeId) == 3){
			GraphSettings * grs = GS((*it));

			n = xml.addNode("object",			{"configurations", "config", "visualObjects"});
			xml.setNodeValue(n, grs->text);
			xml.setAttribute(n, "type", "graph");
			xml.setAttribute(n, "itemName", grs->itemName);
			xml.setAttribute(n, "x", std::to_string(grs->posx));
			xml.setAttribute(n, "y", std::to_string(grs->posy));
			xml.setAttribute(n, "width", std::to_string(grs->width));
			xml.setAttribute(n, "height", std::to_string(grs->height));

			//Finding theme
			int themeId = -1;
			for(size_t i = 0; i<themes.size(); i++){
				if(grs->barColor == themes[i].gt.graphBarsCol and grs->baseColor == themes[i].gt.graphBaseCol and grs->opacity == themes[i].gt.opacity and grs->barWidth == themes[i].gt.barWidth and grs->barSpace == themes[i].gt.barSpacing){
					//Found!
					xml.setAttribute(n, "useTheme", std::to_string(i));
					themeId = i;
				}
			}
			//Create new theme
			if(themeId == -1){
				ThemeInfo ti;
				GraphTheme gt;
				gt.barSpacing = grs->barSpace;
				gt.barWidth = grs->barWidth;
				gt.graphBarsCol = grs->barColor;
				gt.graphBaseCol = grs->baseColor;
				gt.opacity = grs->opacity;
				ti.gt = gt;
				ti.graph = true;
				themes.push_back(ti);
				xml.setAttribute(n, "useTheme", std::to_string(themes.size()-1));
			}

		}  else if((*typeId) == 4){
			DiskSettings * ds = DS((*it));

			n = xml.addNode("object",			{"configurations", "config", "visualObjects"});
			xml.setNodeValue(n, ds->text);
			xml.setAttribute(n, "type", "disk");
			xml.setAttribute(n, "itemName", ds->itemName);
			xml.setAttribute(n, "x", std::to_string(ds->posx));
			xml.setAttribute(n, "y", std::to_string(ds->posy));
			xml.setAttribute(n, "x_inc", std::to_string(ds->posx_inc));
			xml.setAttribute(n, "y_inc", std::to_string(ds->posy_inc));
			xml.setAttribute(n, "width", std::to_string(ds->width));

			//Finding theme
			int themeId = -1;
			for(size_t i = 0; i<themes.size(); i++){
				if(ds->fontPath == themes[i].ft.path and ds->color == themes[i].ft.color and ds->fontSize == themes[i].ft.size and ds->opacity == themes[i].ft.opacity
					and ds->borderThickness == themes[i].dt.borderThickness and ds->borderColor == themes[i].dt.borderColor and ds->fillColor == themes[i].dt.fillColor and ds->diskOpacity == themes[i].dt.opacity and ds->borderHeight == themes[i].dt.borderHeight){					//Found!
					xml.setAttribute(n, "useTheme", std::to_string(i));
					themeId = i;
				}
			}

			//Create new theme
			if(themeId == -1){
				ThemeInfo ti;

				FontTheme ft;
				ft.color = ds->color;
				ft.opacity = ds->opacity;
				ft.path = ds->fontPath;
				ft.size = ds->fontSize;

				DiskTheme dt;
				dt.borderColor = ds->borderColor;
				dt.borderHeight = ds->borderHeight;
				dt.borderThickness = ds->borderThickness;
				dt.fillColor = ds->fillColor;
				dt.opacity = ds->diskOpacity;

				/*ti.font = true;
				ti.ft = ft;

				ti.disk = true;
				ti.dt = dt;
				*/

				ti.ft = ft;
				ti.dt = dt;
				ti.disk = true;

				themes.push_back(ti);
				xml.setAttribute(n, "useTheme", std::to_string(themes.size()-1));
			}

		}

		++typeId;
	}


	for(size_t i = 0; i<themes.size(); i++){
		n = xml.addNode("theme",			{"configurations", "config"});
		xml.setAttribute(n, "id", std::to_string(i));

		if(themes[i].font == true){ //Creating font node
			Node * cn = xml.addNode("font", n);
			xml.setAttribute(cn, "size", std::to_string(themes[i].ft.size));
			xml.setAttribute(cn, "color", themes[i].ft.color);
			xml.setAttribute(cn, "opacity", std::to_string(themes[i].ft.opacity));
			xml.setNodeValue(cn, themes[i].ft.path);
		} else if(themes[i].graph == true){
			Node * cn = xml.addNode("graphTheme", n);
			xml.setAttribute(cn, "graphBarsCol", themes[i].gt.graphBarsCol);
			xml.setAttribute(cn, "graphBaseCol", themes[i].gt.graphBaseCol);
			xml.setAttribute(cn, "opacity", std::to_string(themes[i].gt.opacity));
			xml.setAttribute(cn, "barWidth", std::to_string(themes[i].gt.barWidth));
			xml.setAttribute(cn, "barSpacing", std::to_string(themes[i].gt.barSpacing));
		} else if(themes[i].disk == true){
			//Font part
			Node * cn = xml.addNode("font", n);
			xml.setAttribute(cn, "size", std::to_string(themes[i].ft.size));
			xml.setAttribute(cn, "color", themes[i].ft.color);
			xml.setAttribute(cn, "opacity", std::to_string(themes[i].ft.opacity));
			xml.setNodeValue(cn, themes[i].ft.path);

			//Disk part
			cn = xml.addNode("diskTheme", n);
			xml.setAttribute(cn, "borderThickness", std::to_string(themes[i].dt.borderThickness));
			xml.setAttribute(cn, "borderColor", themes[i].dt.borderColor);
			xml.setAttribute(cn, "fillColor", themes[i].dt.fillColor);
			xml.setAttribute(cn, "opacity", std::to_string(themes[i].dt.opacity));
			xml.setAttribute(cn, "borderHeight", std::to_string(themes[i].dt.borderHeight));
		}

	}



	system("copy config.xml config_backup.xml");
	xml.save("config.xml");
	HWND hDefView = FindWindowEx(NULL, NULL, NULL, TEXT("Program Manager"));
	HWND worker = GetNextWindow(hDefView, GW_HWNDPREV);
	HWND wconkyhwnd = FindWindowEx(worker, NULL, NULL, TEXT("wconky"));
	std::cout<<wconkyhwnd<<std::endl;
	SendMessage(wconkyhwnd, WM_COMMAND, 0x8004, 0);
}

void Settings::LoadGSettings(XmlParser * parser){
	//Settings
	gs.delay = parser->getNode({"configurations", "config", "settings", "delay"})->getValue<int>();
	gs.monitor = parser->getNode({"configurations", "config", "settings", "monitor"})->getValue<int>();
	gs.backgroundPath = parser->getNode({"configurations", "config", "settings", "background"})->getValue<std::string>();
	if(gs.backgroundPath == "0") gs.backgroundPath = "";
	gs.backgroundAuto = gs.backgroundPath.size() > 0 ? 0 : 1;
	std::string bgMode = parser->getNode({"configurations", "config", "settings", "background"})->getAttr<std::string>("mode");
	if(bgMode == "stretch"){
		gs.backgroundMode = 0;
	} else if(bgMode == "oryginal"){
		gs.backgroundMode = 1;
	} else if(bgMode == "fit-in"){
		gs.backgroundMode = 2;
	} else if(bgMode == "fit-out"){
		gs.backgroundMode = 3;
	}
	gs.bgOpacity = parser->getNode({"configurations", "config", "settings", "background"})->getAttr<int>("opacity");

	//Modules
	gs.cpu = parser->getNode({"configurations", "config", "modules", "cpu"})->getValue<int>();
	gs.ram = parser->getNode({"configurations", "config", "modules", "ram"})->getValue<int>();
	gs.disk = parser->getNode({"configurations", "config", "modules", "disk"})->getValue<int>();
	gs.time = parser->getNode({"configurations", "config", "modules", "time"})->getValue<int>();

	gs.cpuFps = parser->getNode({"configurations", "config", "modules", "cpu"})->getAttr<int>("fps");
	gs.ramFps = parser->getNode({"configurations", "config", "modules", "ram"})->getAttr<int>("fps");
	gs.diskFps = parser->getNode({"configurations", "config", "modules", "disk"})->getAttr<int>("fps");
	gs.timeFps = parser->getNode({"configurations", "config", "modules", "time"})->getAttr<int>("fps");

	//Extension
	Node * ext = parser->getNode({"configurations", "config", "Extensions", "ManageIcons"});
	if(ext != nullptr){
		gs.extOn = true;
		gs.extHK = ext->getAttr<int>("hotkey");
	} else {
		gs.extOn = false;
	}
}

void Settings::LoadLSettings(XmlParser * parser){
	Node * daysLang = parser->getNode({"configurations", "config", "lang", "days"});
	ls.monday = daysLang->getAttr<std::string>("monday");
	ls.tuesday = daysLang->getAttr<std::string>("tuesday");
	ls.wednesday = daysLang->getAttr<std::string>("wednesday");
	ls.thursday = daysLang->getAttr<std::string>("thursday");
	ls.friday = daysLang->getAttr<std::string>("friday");
	ls.saturday = daysLang->getAttr<std::string>("saturday");
	ls.sunday = daysLang->getAttr<std::string>("sunday");

	Node * monthsLang = parser->getNode({"configurations", "config", "lang", "months"});
	ls.jan = monthsLang->getAttr<std::string>("jan");
	ls.feb = monthsLang->getAttr<std::string>("feb");
	ls.mar = monthsLang->getAttr<std::string>("mar");
	ls.apr = monthsLang->getAttr<std::string>("apr");
	ls.may = monthsLang->getAttr<std::string>("may");
	ls.june = monthsLang->getAttr<std::string>("june");
	ls.july = monthsLang->getAttr<std::string>("july");
	ls.aug = monthsLang->getAttr<std::string>("aug");
	ls.sept = monthsLang->getAttr<std::string>("sept");
	ls.oct = monthsLang->getAttr<std::string>("oct");
	ls.nov = monthsLang->getAttr<std::string>("nov");
	ls.dec = monthsLang->getAttr<std::string>("dec");
}


Node * Settings::GetThemeNode(XmlParser * pr, int themeId){
	auto list = pr->getNodes({"configurations", "config", "theme"}, nullptr);
	for(auto it = list.begin(); it!=list.end(); it++){
		int id = (*it)->getAttr<int>("id");
		if(id == themeId){
			return (*it);
		}
	}
	return nullptr;
}


void Settings::LoadTextSettings(TextSettings * ts, Node * node, XmlParser * pr){
	ts->text = node->getValue<std::string>();
	if(ts->text == "0") ts->text = "";
	ts->posx = node->getAttr<int>("x");
	ts->posy = node->getAttr<int>("y");
	ts->alignment = node->getAttr<int>("align");
	int theme = node->getAttr<int>("useTheme");
	Node * usedTheme = GetThemeNode(pr, theme);
	if(usedTheme != nullptr){
		auto fontData = pr->getNodes({"font"}, usedTheme);
		ts->fontSize = (*fontData.begin())->getAttr<int>("size");
		ts->opacity = (*fontData.begin())->getAttr<int>("opacity");
		ts->color = (*fontData.begin())->getAttr<std::string>("color");
		if((*fontData.begin())->getValue<std::string>() == "0"){
			ts->fontPath = "";
		} else {
			ts->fontPath = (*fontData.begin())->getValue<std::string>();
		}
	}
	ts->itemName = node->getAttr<std::string>("itemName");
	if(ts->itemName == "0") ts->itemName = "<nameless>";
}



void Settings::LoadGraphSettings(GraphSettings * gs, Node * node, XmlParser * pr){
	gs->text = node->getValue<std::string>();
	if(gs->text == "0") gs->text = "";
	gs->posx = node->getAttr<int>("x");
	gs->posy = node->getAttr<int>("y");
	gs->width = node->getAttr<int>("width");
	gs->height = node->getAttr<int>("height");

	int theme = node->getAttr<int>("useTheme");
	Node * usedTheme = GetThemeNode(pr, theme);
	if(usedTheme != nullptr){
		auto fontData = pr->getNodes({"graphTheme"}, usedTheme);
		gs->barWidth = (*fontData.begin())->getAttr<int>("barWidth");
		gs->barSpace = (*fontData.begin())->getAttr<int>("barSpacing");
		gs->opacity = (*fontData.begin())->getAttr<int>("opacity");
		gs->barColor = (*fontData.begin())->getAttr<std::string>("graphBarsCol");
		gs->baseColor = (*fontData.begin())->getAttr<std::string>("graphBaseCol");
	}
	gs->itemName = node->getAttr<std::string>("itemName");
	if(gs->itemName == "0") gs->itemName = "<nameless>";
}

void Settings::LoadDiskSettings(DiskSettings * ds, Node * node, XmlParser * pr){
	ds->text = node->getValue<std::string>();
	if(ds->text == "0") ds->text = "";
	ds->posx = node->getAttr<int>("x");
	ds->posy = node->getAttr<int>("y");
	ds->posx_inc = node->getAttr<int>("x_inc");
	ds->posy_inc = node->getAttr<int>("y_inc");
	ds->width = node->getAttr<int>("width");

	int theme = node->getAttr<int>("useTheme");
	Node * usedTheme = GetThemeNode(pr, theme);
	if(usedTheme != nullptr){
		//Get font data
		auto fontData = pr->getNodes({"font"}, usedTheme);
		ds->fontSize = (*fontData.begin())->getAttr<int>("size");
		ds->opacity = (*fontData.begin())->getAttr<int>("opacity");
		ds->color = (*fontData.begin())->getAttr<std::string>("color");
		if((*fontData.begin())->getValue<std::string>() == "0"){
			ds->fontPath = "";
		} else {
			ds->fontPath = (*fontData.begin())->getValue<std::string>();
		}
		//Get DiskTheme data
		auto dData = pr->getNodes({"diskTheme"}, usedTheme);
		ds->borderThickness = (*dData.begin())->getAttr<int>("borderThickness");
		ds->borderColor = (*dData.begin())->getAttr<std::string>("borderColor");
		ds->fillColor = (*dData.begin())->getAttr<std::string>("fillColor");
		ds->diskOpacity = (*dData.begin())->getAttr<int>("opacity");
		ds->borderHeight = (*dData.begin())->getAttr<int>("borderHeight");
	}
	ds->itemName = node->getAttr<std::string>("itemName");
	if(ds->itemName == "0") ds->itemName = "<nameless>";
}

void Settings::LoadItems(XmlParser * parser, EditorWindow * ew){
	std::list<Node*> objects = parser->getNodes({"configurations", "config", "visualObjects", "object"}, nullptr);
	for(auto it = objects.begin(); it!=objects.end(); it++){

		if((*it)->getAttr<std::string>("type") == "text"){
			TextSettings * ts = new TextSettings();
			LoadTextSettings(ts, (*it), parser);
			items.push_back(ts);
		    ew->AddItem((char*)ts->itemName.c_str(), rh->resCnt.at(TXT_BITMAP));
		    itemsTypeId.push_back(2);
		} else if((*it)->getAttr<std::string>("type") == "graph"){
			GraphSettings * gs = new GraphSettings();
			LoadGraphSettings(gs, (*it), parser);
			items.push_back(gs);
		    ew->AddItem((char*)gs->itemName.c_str(), rh->resCnt.at(GRAPH_BITMAP));
			itemsTypeId.push_back(3);
		} else if((*it)->getAttr<std::string>("type") == "disk"){
			DiskSettings * ds = new DiskSettings();
			LoadDiskSettings(ds, (*it), parser);
			items.push_back(ds);
		    ew->AddItem((char*)ds->itemName.c_str(), rh->resCnt.at(DISK_BITMAP));
			itemsTypeId.push_back(4);
		}
	}
}

std::string Settings::ReadEditStr(int resId, HWND hDlg){
	HWND temp_hwnd = GetDlgItem(hDlg, resId);
	char buf[255]; memset(buf, 0, 255);
	GetWindowText(temp_hwnd, buf, 255);
	return std::string(buf);
}

int Settings::ReadInt(int resId, HWND hDlg){
	std::string temp = ReadEditStr(resId, hDlg);
	int out = strtol(temp.c_str(), NULL, 10);
	return out;
}

int Settings::ReadRatio(int resFirst, int resSec, HWND hDlg){
	int out = -1;
	for(int i = 0; i<(resSec-resFirst)+1; i++){
		if(SendDlgItemMessage(hDlg, resFirst+i, BM_GETCHECK, 0, 0) == 1){
			out = i;
			break;
		}
	}
	return out;
}

void Settings::ReadGSettings(HWND hDlg){
	gs.monitor = ReadInt(INPUT_MONITOR_SEL, hDlg);
	gs.delay = ReadInt(INPUT_DELAY, hDlg);
	gs.backgroundAuto = ReadRatio(INPUT_BACKGROUND_AUTO_0, INPUT_BACKGROUND_AUTO_1, hDlg);
	gs.backgroundMode = ReadRatio(INPUT_BACKGROUND_MODE_0, INPUT_BACKGROUND_MODE_3, hDlg);
	gs.backgroundPath = ReadEditStr(INPUT_BACKGROUND_PATH, hDlg);
	gs.bgOpacity = ReadInt(INPUT_BACKGROUND_OPACITY, hDlg);

	gs.cpu = ReadRatio(INPUT_MODULE_CPU_0, INPUT_MODULE_CPU_1, hDlg);
	gs.cpuFps = ReadInt(INPUT_MODULE_CPU_FPS, hDlg);

	gs.ram = ReadRatio(INPUT_MODULE_RAM_0, INPUT_MODULE_RAM_1, hDlg);
	gs.ramFps = ReadInt(INPUT_MODULE_RAM_FPS, hDlg);

	gs.disk = ReadRatio(INPUT_MODULE_DISK_0, INPUT_MODULE_DISK_1, hDlg);
	gs.diskFps = ReadInt(INPUT_MODULE_DISK_FPS, hDlg);

	gs.time = ReadRatio(INPUT_MODULE_TIME_0, INPUT_MODULE_TIME_1, hDlg);
	gs.timeFps = ReadInt(INPUT_MODULE_TIME_FPS, hDlg);

	gs.extOn = ReadRatio(INPUT_LOAD_EXTENSIONS_0, INPUT_LOAD_EXTENSIONS_1, hDlg);
	gs.extHK = ReadInt(INPUT_LOAD_EXTENSIONS_HK, hDlg);
}

void Settings::ReadLSettings(HWND hDlg){
	ls.monday = ReadEditStr(MONDAY,hDlg);
	ls.tuesday = ReadEditStr(TUESDAY,hDlg);
	ls.wednesday = ReadEditStr(WEDNESDAY,hDlg);
	ls.thursday = ReadEditStr(THURSDAY,hDlg);
	ls.friday = ReadEditStr(FRIDAY,hDlg);
	ls.saturday = ReadEditStr(SATURDAY,hDlg);
	ls.sunday = ReadEditStr(SUNDAY,hDlg);
	ls.jan = ReadEditStr(JANUARY,hDlg);
	ls.feb = ReadEditStr(FEBRUARY,hDlg);
	ls.mar = ReadEditStr(MARCH,hDlg);
	ls.apr = ReadEditStr(APRIL,hDlg);
	ls.may = ReadEditStr(MAY,hDlg);
	ls.june = ReadEditStr(JUNE,hDlg);
	ls.july = ReadEditStr(JULY,hDlg);
	ls.aug = ReadEditStr(AUGUST,hDlg);
	ls.sept = ReadEditStr(SEPTEMBER,hDlg);
	ls.oct = ReadEditStr(OCTOBER,hDlg);
	ls.nov = ReadEditStr(NOVEMBER,hDlg);
	ls.dec = ReadEditStr(DECEMBER,hDlg);
}


void Settings::ReadTextSettings(int itemId, HWND hDlg){
	std::list<void*>::iterator it = items.begin();
	std::advance(it, itemId);
	TextSettings * ts = TS((*it));

	ts->itemName = ReadEditStr(INPUT_T_NAME, hDlg);
	ts->text = ReadEditStr(INPUT_T_TEXT, hDlg);
	ts->posx = ReadInt(INPUT_T_XPOS, hDlg);
	ts->posy = ReadInt(INPUT_T_YPOS, hDlg);
	ts->alignment = ReadRatio(INPUT_T_ALIG_0, INPUT_T_ALIG_2, hDlg);
	ts->fontPath = ReadEditStr(INPUT_T_FONT_P_T,hDlg);
	ts->color = ReadEditStr(INPUT_T_FONT_C,hDlg);
	ts->fontSize = ReadInt(INPUT_T_FONT_S, hDlg);
	ts->opacity = ReadInt(INPUT_T_FONT_O, hDlg);
}


void Settings::ReadGraphSettings(int itemId, HWND hDlg){
	std::list<void*>::iterator it = items.begin();
	std::advance(it, itemId);
	GraphSettings * grs = GS((*it));

	grs->itemName = ReadEditStr(INPUT_G_NAME, hDlg);
	grs->text = ReadEditStr(INPUT_G_TEXT, hDlg);

	grs->posx = ReadInt(INPUT_G_XPOS, hDlg);
	grs->posy = ReadInt(INPUT_G_YPOS, hDlg);
	grs->width = ReadInt(INPUT_G_WIDTH, hDlg);
	grs->height = ReadInt(INPUT_G_HEIGHT, hDlg);

	grs->baseColor = ReadEditStr(INPUT_G_BASE_C, hDlg);
	grs->barColor = ReadEditStr(INPUT_G_BAR_C, hDlg);

	grs->barWidth = ReadInt(INPUT_G_BAR_W, hDlg);
	grs->barSpace = ReadInt(INPUT_G_BAR_SPACE, hDlg);
	grs->opacity = ReadInt(INPUT_G_OPACITY, hDlg);
}


void Settings::ReadDiskSettings(int itemId, HWND hDlg){

	std::list<void*>::iterator it = items.begin();
	std::advance(it, itemId);
	DiskSettings * ds = DS((*it));

	ds->itemName = ReadEditStr(INPUT_D_NAME, hDlg);
	ds->text = ReadEditStr(INPUT_D_TEXT, hDlg);
	ds->posx = ReadInt(INPUT_D_XPOS, hDlg);
	ds->posy = ReadInt(INPUT_D_YPOS, hDlg);
	ds->posx_inc = ReadInt(INPUT_D_ADVX, hDlg);
	ds->posy_inc = ReadInt(INPUT_D_ADVY, hDlg);
	ds->width = ReadInt(INPUT_D_WIDTH, hDlg);
	ds->fontPath = ReadEditStr(INPUT_D_FONT_P, hDlg);
	ds->color = ReadEditStr(INPUT_D_FONT_C, hDlg);
	ds->fontSize = ReadInt(INPUT_D_FONT_S, hDlg);
	ds->opacity = ReadInt(INPUT_D_FONT_O, hDlg);
	ds->borderThickness = ReadInt(INPUT_D_BT, hDlg);
	ds->borderColor = ReadEditStr(INPUT_D_BC, hDlg);
	ds->fillColor =	ReadEditStr(INPUT_D_FC, hDlg);
	ds->diskOpacity = ReadInt(INPUT_D_DO, hDlg);
	ds->borderHeight = ReadInt(INPUT_D_BH, hDlg);
}
