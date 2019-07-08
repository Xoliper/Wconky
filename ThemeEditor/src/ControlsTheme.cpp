/*
 * ControlsTheme.cpp
 *
 *  Created on: 20 kwi 2018
 *      Author: Xoliper
 */

#include "ControlsTheme.hpp"


//BUTTON THEME OVERRIDE
void DrawHeaderBtn(DRAWITEMSTRUCT * dis, HWND hwnd){

	//Get Rect
    RECT rc;
    GetClientRect( hwnd, &rc );
    int height = rc.bottom-rc.top;

    //Fill Background
	if (dis->itemState & ODS_SELECTED) { //Clicked
		FillRect(dis->hDC, &rc, (HBRUSH) (COLOR_WINDOW));
    } else {
		FillRect(dis->hDC, &rc, (HBRUSH) (COLOR_WINDOW+1));
    }

	//Draw ICON
    BITMAP bm;

	HBITMAP hbmpPicture2 = (HBITMAP)SendMessage(dis->hwndItem, BM_GETIMAGE, IMAGE_BITMAP, 0);

	HDC hdcMem = CreateCompatibleDC(dis->hDC);
    HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem,hbmpPicture2);

    GetObject(hbmpPicture2, sizeof(bm), &bm);

    TransparentBlt (dis->hDC, 10, (height-bm.bmHeight)/2, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255,255,255));

    SelectObject(hdcMem, hbmOld);
    DeleteDC(hdcMem);


	//Set Text
    SetBkMode(dis->hDC, TRANSPARENT );
    char buf[255];
    GetWindowText(hwnd, buf, 255);
    rc.left +=  bm.bmWidth + 25;
    rc.top -= 3;
    DrawText(dis->hDC, buf, strlen(buf), &rc, DT_VCENTER|DT_SINGLELINE );

}


