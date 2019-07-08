/*
 * ResLoader.cpp
 *
 *  Created on: 20 kwi 2018
 *      Author: Xoliper
 */

#include "ResHolder.hpp"

ResHolder::ResHolder(HWND hwnd, HINSTANCE hInstance) {

	this->hwnd = hwnd;
	hInst = hInstance;

	//Load bitmaps
	HBITMAP temp = NULL;
	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(BTN_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load BTN_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(BTN_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(HOME_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load HOME_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(HOME_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(GSETTINGS_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load GSETTINGS_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(GSETTINGS_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(TXT_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load TXT_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(TXT_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(GRAPH_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load TXT_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(GRAPH_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(TXTADD_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load TXT_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(TXTADD_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(GRAPHADD_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load TXT_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(GRAPHADD_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(DELETE_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load DELETE_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(DELETE_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(LSETTINGS_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load DELETE_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(LSETTINGS_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(SAVE_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load SAVE_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(SAVE_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(DISKADD_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load DISK_ADD_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(DISKADD_BITMAP, temp));
	temp = NULL;

	temp = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(DISK_BITMAP));
	if(temp == NULL)
		MessageBox(NULL, "Could not load DISK_BITMAP!", "Error", MB_OK | MB_ICONEXCLAMATION);
	resCnt.insert(std::pair<int, HBITMAP>(DISK_BITMAP, temp));
	temp = NULL;

	//Load Brushes
    brushCnt.insert(std::pair<int, HBRUSH>(BRUSH_WHITE, CreateSolidBrush(RGB(255,255,222))));
    brushCnt.insert(std::pair<int, HBRUSH>(BRUSH_BLUE, CreateSolidBrush(RGB(0,150,215))));
    brushCnt.insert(std::pair<int, HBRUSH>(BRUSH_GRAY, CreateSolidBrush(RGB(150,150,150))));

    //Load Fonts
	//Create fonts
	HFONT fTemp = NULL;

	fTemp = CreateFont(
			40, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
	fontCnt.insert(std::pair<int, HFONT>(FONT_BIG, fTemp));

	fTemp = CreateFont(
			24, 0, 0, 0, FW_LIGHT, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
	fontCnt.insert(std::pair<int, HFONT>(FONT_SMALL, fTemp));

}

ResHolder::~ResHolder() {

	//DeleteObject(asdsad);
}

