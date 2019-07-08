/*
 * winopacity.hpp
 *
 *  Created on: 30 wrz 2016
 *      Author: Arkadiusz Jarlaczyk
 */

#ifndef WINOPACITY_HPP_
#define WINOPACITY_HPP_

#include <windows.h>
#include <string>
#include <iostream>

std::string GetWallpaperPath();

BOOL CALLBACK CountMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
BOOL CALLBACK IterateMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

class MonitorInfo{
	friend BOOL CALLBACK CountMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);
	friend BOOL CALLBACK IterateMonitors(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

public:
	MonitorInfo();
	MonitorInfo(int monitor_id);
	~MonitorInfo();

	RECT GetMonitorRect();
	int MonitorsAmount();
	int GetMinX();
	int GetMinY();

private:
	RECT data;
	int check;
	int selected;
	int min_x, min_y;
	int monitors;
};


#endif /* WINOPACITY_HPP_ */
