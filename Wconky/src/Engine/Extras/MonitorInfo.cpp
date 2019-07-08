/*
 * winopacity.cpp
 *
 *  Created on: 3 pa« 2016
 *      Author: Arkadiusz Jarlaczyk
 */

#include "MonitorInfo.hpp"

std::string GetWallpaperPath(){
	char path[255];
	SystemParametersInfo(SPI_GETDESKWALLPAPER, 255, path, 0);
	return std::string(path);
}

BOOL CALLBACK CountMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData){
	MonitorInfo * mi = (MonitorInfo*)dwData;
	mi->monitors += 1;
    return TRUE;
}

BOOL CALLBACK IterateMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData){

	MonitorInfo * mi = (MonitorInfo*)dwData;

	//Fix multimonitor
	RECT temp = *lprcMonitor;
	if(mi->min_x > temp.left){
		mi->min_x = temp.left;
	}
	if(mi->min_y > temp.top){
		mi->min_y = temp.top;
	}

	if(mi->selected != mi->check){
		++mi->check;
		return true;
	}
	++mi->check;

	mi->data = *lprcMonitor;

    return TRUE;
}

MonitorInfo::MonitorInfo(){
	check = 0;
	selected = 0;
	min_x = min_y = 0;
	monitors = 0;
}

MonitorInfo::MonitorInfo(int monitor_id){
	check = 0;
	selected = monitor_id;
	min_x = min_y = 0;
	monitors = 0;
}

MonitorInfo::~MonitorInfo(){
}

RECT MonitorInfo::GetMonitorRect(){
	check = 0;
	HDC hdc = GetDC(nullptr);
	EnumDisplayMonitors(hdc, nullptr, IterateMonitors, (LPARAM)this);
	return data;
}

int MonitorInfo::MonitorsAmount(){
	check = 0;
	monitors = 0;
	HDC hdc = GetDC(nullptr);
	EnumDisplayMonitors(hdc, nullptr, CountMonitors, (LPARAM)this);
	return monitors;
}

int MonitorInfo::GetMinX(){
	return min_x;
}

int MonitorInfo::GetMinY(){
	return min_y;
}
