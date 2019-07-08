/*
 * EditorWindow.cpp
 *
 *  Created on: 30 mar 2018
 *      Author: Xoliper
 */

#include "EditorWindow.hpp"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


    PAINTSTRUCT ps;
    HDC hDC;

    if(ew != NULL){

    switch(msg)
    {

    case WM_COMMAND: {

    	if(ew != NULL){

			int count = GetListBoxInfo(ew->hListBox);

			if (LOWORD(wParam) == BTN_ADDTXT) {
				ew->AddItem("<Enter Name>", rh->resCnt.at(TXT_BITMAP), true);
				SendMessage(ew->hListBox, LB_SETCURSEL, count, lParam);
			} else if(LOWORD(wParam) == BTN_ADDGRAPH){
				ew->AddItem("<Enter Name>", rh->resCnt.at(GRAPH_BITMAP), true);
				SendMessage(ew->hListBox, LB_SETCURSEL, count, lParam);
			} else if(LOWORD(wParam) == BTN_ADDDISK){
				ew->AddItem("<Enter Name>", rh->resCnt.at(DISK_BITMAP), true);
				SendMessage(ew->hListBox, LB_SETCURSEL, count, lParam);
			} else if(LOWORD(wParam) == BTN_DELETE){

				for(int i = 0; i<count; i++){
					int ifSelected = SendMessage(ew->hListBox, LB_GETSEL, i, 0);
					if(ifSelected and i != 0 and i != 1){
						SendMessage(ew->hListBox, LB_SETCURSEL, i-1, lParam);
						SendMessage(ew->hListBox,(UINT) LB_DELETESTRING,i,lParam);

						std::list<void*>::iterator it = s->items.begin();
						std::advance(it, i);
						s->items.erase(it);

						std::list<int>::iterator itType = s->itemsTypeId.begin();
						std::advance(itType, i);
						s->itemsTypeId.erase(itType);

						break;
					}
				}

			} else if(LOWORD(wParam) == BTN_SAVE){
				ew->SaveItemData();
				s->SaveAsXmlConfig("Path");
			}
    	}

    } break;

    case WM_PAINT: {
        hDC = BeginPaint(hwnd, &ps);

        RECT rc;
        GetClientRect( hwnd, &rc );
        rc.left = 10;
    	rc.right -= 10;
    	rc.top = 60;
    	rc.bottom = 62;
		FillRect(hDC, &rc, rh->brushCnt.at(BRUSH_BLUE));
    	rc.top = 120;
    	rc.bottom = 122;

		GetClientRect( hwnd, &rc );
        rc.left = 300;
    	rc.right = 302;
    	rc.top = 62;
    	rc.bottom -= 10;
		FillRect(hDC, &rc, rh->brushCnt.at(BRUSH_BLUE));


		EndPaint(hwnd, &ps);

    } return 0L;

    case WM_SIZE: {
        InvalidateRect(hwnd, NULL, TRUE);
    } break;



    case WM_CTLCOLORSTATIC:
    	SetBkMode((HDC)wParam,TRANSPARENT);
    	SetBkColor((HDC)wParam, RGB(0,0,0));
    	SetTextColor((HDC)wParam, RGB(0,0,0));
    return (INT_PTR)(COLOR_WINDOW+1);


    case WM_DRAWITEM: {

    	DRAWITEMSTRUCT *dis = (DRAWITEMSTRUCT*)lParam;

    	//ListBox
    	switch (dis->itemAction)
    	{


    		case ODA_SELECT:
    	    case ODA_DRAWENTIRE: {


    			FillRect(dis->hDC, &dis->rcItem, (HBRUSH) (COLOR_WINDOW+1));
    	    	// Draw the bitmap associated with the item.
    	    	// Get the item bitmap
    	    	HBITMAP hbmpPicture = (HBITMAP)SendMessage(dis->hwndItem, LB_GETITEMDATA, dis->itemID, 0);

    	    	BITMAP bm;
    	        // Create a compatible device context.
    	    	HDC hdcMem = CreateCompatibleDC(dis->hDC);
    	    	// Select the item bitmap into the compatible device
    	    	// context and save the old bitmap.
    	    	HBITMAP hbmpOld = (HBITMAP) SelectObject(hdcMem, hbmpPicture);

    	    	GetObject(rh->resCnt.at(BTN_BITMAP), sizeof(bm), &bm);



    	    	//DRAWING SECTIONS
            	RECT rc = dis->rcItem;
    			int height = rc.bottom -rc.top;
    			int i = 0;
    			int ifSelected = 0;

    	    	// Is the item selected?
                if (dis->itemState & ODS_SELECTED)
                {
                	if(ew != NULL){
        				int count = GetListBoxInfo(ew->hListBox);

        				for(; i<count; i++){
        					ifSelected = SendMessage(ew->hListBox, LB_GETSEL, i, 0);
        					if(ifSelected){
        						ew->setCurSel(i);
        						if(ew->helper != i){
        							ew->LoadForm(i);
        							ew->helper = i;
        						}


        						break;
        					}
        				}
                	}

                	rc.right = rc.left+10;
        			FillRect(dis->hDC, &rc, rh->brushCnt.at(BRUSH_BLUE));

        		    TransparentBlt (dis->hDC, dis->rcItem.left+15, dis->rcItem.top+(height-bm.bmHeight)/2, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255,255,255));
					TCHAR achBuffer[255];
					SendMessage(dis->hwndItem, LB_GETTEXT, dis->itemID, (LPARAM)achBuffer);
					dis->rcItem.left +=  bm.bmWidth + 28;
			    	SetTextColor(dis->hDC, RGB(0,150,215));

					DrawText(dis->hDC, achBuffer, strlen(achBuffer), &dis->rcItem, DT_VCENTER|DT_SINGLELINE );

                } else {

        		    TransparentBlt (dis->hDC, dis->rcItem.left+15, dis->rcItem.top+(height-bm.bmHeight)/2, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, RGB(255,255,255));
					TCHAR achBuffer[255];
					SendMessage(dis->hwndItem, LB_GETTEXT, dis->itemID, (LPARAM)achBuffer);
					dis->rcItem.left +=  bm.bmWidth + 28;
			    	SetTextColor(dis->hDC, RGB(0,0,0));
					DrawText(dis->hDC, achBuffer, strlen(achBuffer), &dis->rcItem, DT_VCENTER|DT_SINGLELINE );
                }

                rc.left = 10;
                rc.right = 12;
    			FillRect(dis->hDC, &rc,rh->brushCnt.at(BRUSH_BLUE));

    	    	// Clean up.
    	    	SelectObject(hdcMem, hbmpOld);
    	    	DeleteDC(hdcMem);

    	    } break;

    	}

    	if(dis->CtlType == ODT_BUTTON){
    		DrawHeaderBtn(dis, dis->hwndItem);
    	}

    	return TRUE;
    }

    case WM_MEASUREITEM:
    	PMEASUREITEMSTRUCT pmis;
        pmis = (PMEASUREITEMSTRUCT) lParam;
        pmis->itemHeight = 35;//YBITMAP;
    return true;

    case WM_CLOSE:
    	DestroyWindow(hwnd);
    break;

    case WM_DESTROY:
    	PostQuitMessage(0);
    break;

    default:
    	return DefWindowProc(hwnd, msg, wParam, lParam);
    }

	} else {
    	return DefWindowProc(hwnd, msg, wParam, lParam);
	}

    return 0;
}


LRESULT CALLBACK WndProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    PAINTSTRUCT ps;
    HDC hDC;

    switch(msg)
    {


    case WM_COMMAND: {

    	if(ew != NULL){


    		if((LOWORD(wParam) == INPUT_T_FONT_C or LOWORD(wParam) == INPUT_G_BASE_C or LOWORD(wParam) == INPUT_G_BAR_C or LOWORD(wParam) == INPUT_D_FC or LOWORD(wParam) == INPUT_D_BC or LOWORD(wParam) == INPUT_D_FONT_C) and HIWORD(wParam) == BN_CLICKED){
				CHOOSECOLOR ccl;
				BOOL bResult;

				ZeroMemory( & ccl, sizeof( CHOOSECOLOR ) );
				ccl.lStructSize = sizeof( CHOOSECOLOR );
				ccl.hwndOwner = hwnd;
				ccl.lpCustColors = Colors;
				ccl.Flags = CC_ANYCOLOR;

				bResult = ChooseColor( & ccl );
				if( bResult ){
					char hexcol[7];
					memset(hexcol, '\0', 7);
					snprintf(hexcol, 7, "%02X%02X%02X", GetRValue( ccl.rgbResult ), GetGValue( ccl.rgbResult ), GetBValue( ccl.rgbResult ));
					std::string finalColor = "0x"+std::string(hexcol);
					HWND tempHwnd = GetDlgItem(hwnd, LOWORD(wParam));
					SetWindowText(tempHwnd, finalColor.c_str());
				}
    		}


    		//Select font & background combobox
    		if((LOWORD(wParam) == INPUT_T_FONT_P_T or LOWORD(wParam) == INPUT_D_FONT_P) and HIWORD(wParam) == CBN_SELCHANGE){

    			HWND tempHwnd = GetDlgItem(hwnd, LOWORD(wParam));
    			int cbsel = SendMessage(tempHwnd, CB_GETCURSEL, 0, 0);

    			if(cbsel == 0){

					OPENFILENAME ofn;
					char sNazwaPliku[ MAX_PATH ] = "";

					ZeroMemory( & ofn, sizeof( ofn ) );
					ofn.lStructSize = sizeof( ofn );
					ofn.lpstrFilter = "TrueType Fonts (*.ttf)\0*.ttf\0";
					ofn.nMaxFile = MAX_PATH;
					ofn.lpstrFile = sNazwaPliku;
					ofn.lpstrDefExt = "ttf\0";
					//ofn.lpstrInitialDir = "C:\\Windows\\Fonts\\";
					ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
					if( GetOpenFileName( & ofn ) )
					{
						std::cout<<ofn.lpstrFile<<std::endl;
						SendMessage(tempHwnd,(UINT) CB_DELETESTRING, 1, 0);
			    		SendMessage(tempHwnd, CB_INSERTSTRING, 1,( LPARAM ) ofn.lpstrFile );
						SendMessage(tempHwnd, CB_SETCURSEL, 1, lParam);
					}
    			}
    		} else if(LOWORD(wParam) == INPUT_BACKGROUND_PATH and HIWORD(wParam) == CBN_SELCHANGE){

    			HWND tempHwnd = GetDlgItem(hwnd, INPUT_BACKGROUND_PATH);
    			int cbsel = SendMessage(tempHwnd, CB_GETCURSEL, 0, 0);

    			if(cbsel == 0){
					OPENFILENAME ofn;
					char sNazwaPliku[ MAX_PATH ] = "";

					ZeroMemory( & ofn, sizeof( ofn ) );
					ofn.lStructSize = sizeof( ofn );
					ofn.lpstrFilter = "All Image files (*.jpg;*.jpeg;*.png;*.bmp)\0*.jpg;*.jpeg;*.png;*.bmp\0"
										"Image files (*.jpg)\0*.jpg\0;"
										"Image files (*.jpeg)\0*.jpeg\0"
										"Image files (*.png)\0*.png\0"
										"Image files (*.bmp)\0*.bmp\0";
					ofn.nMaxFile = MAX_PATH;
					ofn.lpstrFile = sNazwaPliku;
					ofn.lpstrDefExt = "bmp\0";
					//ofn.lpstrInitialDir = "C:\\Windows\\Fonts\\";
					ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
					if( GetOpenFileName( & ofn ) )
					{
						std::cout<<ofn.lpstrFile<<std::endl;
						SendMessage(tempHwnd,(UINT) CB_DELETESTRING, 1, 0);
			    		SendMessage(tempHwnd, CB_INSERTSTRING, 1,( LPARAM ) ofn.lpstrFile );
						SendMessage(tempHwnd, CB_SETCURSEL, 1, lParam);
					}
    			}
    		}


    		//RATIO
    		if(HIWORD(wParam) == BN_CLICKED){
    			for(unsigned int i = 0; i<ew->ratioData.size(); i++){
    				if(LOWORD(wParam) >=ew->ratioData[i].start and LOWORD(wParam) <= ew->ratioData[i].end){
    	                for(int j = 0; j<(ew->ratioData[i].end-ew->ratioData[i].start)+1; j++){
    	                    SendDlgItemMessage(hwnd, ew->ratioData[i].start+j, BM_SETCHECK, 0, 0);
    	                }
    	    			SendDlgItemMessage(hwnd, LOWORD(wParam), BM_SETCHECK, 1, 0);
    					break;
    				}
    			}
    		}

    		//LISTBOX
    		if((LOWORD(wParam) == INPUT_T_NAME || LOWORD(wParam) == INPUT_G_NAME || LOWORD(wParam) == INPUT_D_NAME)and HIWORD(wParam) == EN_CHANGE){


    			int size = SendMessage(ew->hListBox, LB_GETTEXTLEN , ew->curSel, 0);
    			char temp[size];
    			SendMessage(ew->hListBox, LB_GETTEXT , ew->curSel, (LPARAM)temp);

    			//Get delegate
    			HWND editControl = GetDlgItem(hwnd, LOWORD(wParam));
    			char changeTo[255];
    			GetWindowText(editControl, changeTo, 255);

    			if(strcmp(temp,changeTo) != 0){
    				ew->helper = ew->curSel;
    				HBITMAP htemp = (HBITMAP) SendMessage(ew->hListBox, LB_GETITEMDATA, ew->curSel, 0);
    				SendMessage(ew->hListBox, LB_DELETESTRING, ew->curSel, 0);
    		    	SendMessage(ew->hListBox, LB_INSERTSTRING, ew->curSel, (LPARAM)changeTo);
    		    	SendMessage(ew->hListBox, LB_SETITEMDATA, (WPARAM)ew->curSel, (LPARAM)htemp);
    		    	SendMessage(ew->hListBox, LB_SETCURSEL, ew->curSel, 0);
    			}

    			if(strcmp(temp,changeTo) != 0){

    				HBITMAP htemp = (HBITMAP) SendMessage(ew->hListBox, LB_GETITEMDATA, ew->curSel, 0);
    				SendMessage(ew->hListBox, LB_DELETESTRING, ew->curSel, 0);
    		    	SendMessage(ew->hListBox, LB_INSERTSTRING, ew->curSel, (LPARAM)changeTo);
    		    	SendMessage(ew->hListBox, LB_SETITEMDATA, (WPARAM)ew->curSel, (LPARAM)htemp);
    			}
		    	SetFocus(editControl);
    		}

    	}

    } break;


    case WM_SIZE: {
        InvalidateRect(hwnd, NULL, TRUE);
    } break;


    case WM_CTLCOLORSTATIC:
    	SetBkMode((HDC)wParam,TRANSPARENT);
    	SetTextColor((HDC)wParam, RGB(0,0,0));
    return (INT_PTR)(COLOR_WINDOW+1);

   // case WM_CTLCOLORBTN:
    //	{
      //  	SetBkMode((HDC)wParam,TRANSPARENT);
    	//	//SetBkColor((HDC)wParam, RGB(255, 0, 0));
    		//return (LONG);//rh->brushCnt.at(BRUSH_BLUE);
           // }

    case WM_DRAWITEM: {
    	DRAWITEMSTRUCT *dis = (DRAWITEMSTRUCT*)lParam;

    	if(dis->CtlType == ODT_BUTTON){
    	 DrawHeaderBtn(dis, dis->hwndItem);
    	}

    	return TRUE;
    }


    case WM_PAINT: {
        hDC = BeginPaint(hwnd, &ps);

        /*RECT rc;

		GetClientRect( hwnd, &rc );
        rc.left = 230;
    	rc.right = 232;
    	rc.top = 0;
    	rc.bottom = ew->editorLineHeight-ew->fixTip;
		FillRect(hDC, &rc, rh->brushCnt.at(BRUSH_GRAY));

		GetClientRect( hwnd, &rc );
    	rc.top = ew->editorLineHeight-ew->fixTip;
		FillRect(hDC, &rc, rh->brushCnt.at(BRUSH_BLUE));
*/
		EndPaint(hwnd, &ps);

    } return 0L;

    case WM_VSCROLL: {

        SCROLLINFO si;
        ZeroMemory( & si, sizeof( si ) );
        si.cbSize = sizeof( SCROLLINFO );
        si.fMask = SIF_POS | SIF_PAGE | SIF_TRACKPOS;
        GetScrollInfo( hwnd, SB_VERT, & si );

        int pozycja = si.nPos;

        switch( LOWORD( wParam ) ) {
        case SB_TOP:
            pozycja = 0;
            break;
        case SB_BOTTOM:
            pozycja = ew->editorLineHeight;
            break;
        case SB_LINEUP:
            if( pozycja > 0 ) {
                pozycja--;
            }
            break;
        case SB_LINEDOWN:
            if( pozycja < ew->editorLineHeight ) {
                pozycja++;
            }
            break;
        case SB_PAGEUP:
            pozycja -= si.nPage;
            if( pozycja < 0 ) {
                pozycja = 0;
            }
            break;
        case SB_PAGEDOWN:
            pozycja += si.nPage;
            if( pozycja > ew->editorLineHeight ) {
                pozycja = ew->editorLineHeight;
            }
            break;
        case SB_THUMBPOSITION:
            pozycja = si.nTrackPos;
            break;
        case SB_THUMBTRACK:
            pozycja = si.nTrackPos;
            break;
        }

        int dy = -( pozycja - si.nPos );
        ScrollWindowEx( hwnd, 0, dy,( CONST RECT * ) NULL,( CONST RECT * ) NULL,( HRGN ) NULL,( LPRECT ) NULL, SW_SCROLLCHILDREN | SW_INVALIDATE | SW_ERASE );

        UpdateWindow( hwnd );

        ZeroMemory( & si, sizeof( si ) );
        si.cbSize = sizeof( SCROLLINFO );
        si.fMask = SIF_POS;
        si.nPos = pozycja;

        SetScrollInfo( hwnd, SB_VERT, & si, TRUE );
    }
    break;

    case WM_CLOSE:
    	DestroyWindow(hwnd);
    break;

    case WM_DESTROY:
    break;

    default:
    	return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}



EditorWindow::EditorWindow(HINSTANCE hInstance, HWND hwnd) {


	hInst = hInstance;
	ohwnd = hwnd;

	//Create objects
	RECT rc;
	GetClientRect( hwnd, &rc );

	//Add labels
	HWND OBJ_LABEL = CreateWindowEx(0, "STATIC", "Objects", WS_CHILD | WS_VISIBLE, 10, 80, 200, 40, hwnd, (HMENU)1000, hInst, NULL);
	SendMessage(OBJ_LABEL, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);

	HWND EDIT_LABEL = CreateWindowEx(0, "STATIC", "Settings", WS_CHILD | WS_VISIBLE, 325, 80, 200, 40, hwnd, (HMENU)1001, hInst, NULL);
	SendMessage(EDIT_LABEL, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);

	//Create buttons
	button = CreateWindow(
			"BUTTON", "Theme Editor",  WS_VISIBLE | WS_CHILD |  BS_BITMAP | BS_OWNERDRAW,
			10, 10, 240, 40, hwnd, (HMENU)BTN_BACK, hInst, nullptr);

	//Create buttons
	saveButton = CreateWindow(
			"BUTTON", "Save Config",  WS_VISIBLE | WS_CHILD |  BS_BITMAP | BS_OWNERDRAW,
			10+240+65, 14, 150, 40, hwnd, (HMENU)BTN_SAVE, hInst, nullptr);

	//Create buttons
	addTxtBtn = CreateWindowEx(
			0, "BUTTON", "", WS_VISIBLE | WS_CHILD |  BS_BITMAP |  BS_OWNERDRAW ,
			290-93-10-50-48, 76, 53, 32, hwnd, (HMENU)BTN_ADDTXT, hInst, nullptr);

	//Create buttons
	addGraphBtn = CreateWindowEx(
			0, "BUTTON", "", WS_VISIBLE | WS_CHILD | BS_BITMAP |  BS_OWNERDRAW ,
			290-45-10-50, 76, 47, 32, hwnd, (HMENU)BTN_ADDGRAPH, hInst, nullptr);

	//Create buttons
	addDiskBtn = CreateWindowEx(
			0, "BUTTON", "", WS_VISIBLE | WS_CHILD | BS_BITMAP |  BS_OWNERDRAW ,
			290-45-10-50-48, 76, 47, 32, hwnd, (HMENU)BTN_ADDDISK, hInst, nullptr);

	//Create buttons
	deleteBtn = CreateWindowEx(
			0, "BUTTON", "", WS_VISIBLE | WS_CHILD |BS_BITMAP |   BS_OWNERDRAW ,
			290-45-10, 76, 47, 32, hwnd, (HMENU)BTN_DELETE, hInst, nullptr);

	//Create ListBox
	hListBox = CreateWindowEx( 0, "LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_OWNERDRAWVARIABLE   | LBS_HASSTRINGS | LBS_NOTIFY,
			10, 123, 290, rc.bottom-150, hwnd, (HMENU)LIST_BOX, hInst, NULL );
	ListView_SetExtendedListViewStyle( hListBox, LVS_EX_FULLROWSELECT | LVS_EX_FLATSB | LVS_EX_TWOCLICKACTIVATE); //LVN_ITEMACTIVATE


	SetFocus(hListBox);
	SendMessage(hListBox, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);


	//Apply theme
	SendMessage(button, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_BIG), TRUE);
	HBITMAP hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(BTN_BITMAP));
	SendMessage(button, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
	SendMessage(addTxtBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)rh->resCnt.at(TXTADD_BITMAP));
	SendMessage(addGraphBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)rh->resCnt.at(GRAPHADD_BITMAP));
	SendMessage(deleteBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)rh->resCnt.at(DELETE_BITMAP));
	SendMessage(saveButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)rh->resCnt.at(SAVE_BITMAP));
	SendMessage(addDiskBtn, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)rh->resCnt.at(DISKADD_BITMAP));
	SendMessage(saveButton, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);


	curSel = -1;
	curSelType = GSETTINGS;
	righthwnd = NULL;
	fixTip = 0;

	InvalidateRect(hwnd, NULL, TRUE);

	//Zero colors
	for(int i = 0; i<16;i++){
		Colors[i] = RGB(0,0,0);
	}

}

EditorWindow::~EditorWindow() {
}


void EditorWindow::AddItem(char pstr [], HBITMAP hbmp, bool mode)
{

	if(hbmp == (HBITMAP)rh->resCnt.at(GSETTINGS_BITMAP)){
		s->items.push_back(&s->gs);
		s->itemsTypeId.push_back(0);
	}  else if(hbmp == (HBITMAP)rh->resCnt.at(TXT_BITMAP)){
		if(mode == true){
			TextSettings * ts = new TextSettings();
			s->items.push_back(ts);
			s->itemsTypeId.push_back(2);
		}
	} else if(hbmp == (HBITMAP)rh->resCnt.at(GRAPH_BITMAP)){
		if(mode == true){
			GraphSettings * gs = new GraphSettings();
			s->items.push_back(gs);
			s->itemsTypeId.push_back(3);
		}
	} else if(hbmp == rh->resCnt.at(LSETTINGS_BITMAP)){
		s->items.push_back(&s->ls);
		s->itemsTypeId.push_back(1);
	} else if(hbmp == rh->resCnt.at(DISK_BITMAP)){
		if(mode == true){
			DiskSettings * ds = new DiskSettings();
			s->items.push_back(ds);
			s->itemsTypeId.push_back(4);
		}
	}

    int lbItem;
    lbItem = SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)pstr);
    SendMessage(hListBox, LB_SETITEMDATA, (WPARAM)lbItem, (LPARAM)hbmp);
    InvalidateRect(hListBox, NULL, TRUE);
    UpdateWindow(hListBox);
}

void EditorWindow::SelectFirst(){
	   SendMessage(hListBox, LB_SETCURSEL, 0, 0);
}


void EditorWindow::SaveItemData(){
	//Save last form data first!
	if(righthwnd != NULL){
		if(helper == 0){
			s->ReadGSettings(righthwnd);
		} else if(helper == 1){
			s->ReadLSettings(righthwnd);
		} else {
			HBITMAP hb = (HBITMAP)SendMessage(hListBox, LB_GETITEMDATA, helper, 0);
			if(hb == (HBITMAP)rh->resCnt.at(TXT_BITMAP)){
				s->ReadTextSettings(helper, righthwnd);
			} else if(hb == (HBITMAP)rh->resCnt.at(GRAPH_BITMAP)){
				s->ReadGraphSettings(helper, righthwnd);
			} else if(hb == (HBITMAP)rh->resCnt.at(DISK_BITMAP)){
				s->ReadDiskSettings(helper, righthwnd);
			}
		}
	}
}

void EditorWindow::LoadForm(int sel){

	//Save last form data first!
	ew->SaveItemData();

	//Cleanup
	ew->fixTip = 0;
	ew->ratioData.clear();
	DestroyWindow(righthwnd);
	righthwnd = NULL;

	//--------------------------------------------------
	//RIGHT WINDOW PART
	//--------------------------------------------------
	righthwnd = CreateWindowEx(
        0,
        "Edit Window Item",
        "Edit Window Item",
         WS_CHILDWINDOW | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL,
		325, 120, 650, 545, //650, 500
        ohwnd, (HMENU)6060, hInst, NULL);

    if(righthwnd == nullptr){
        MessageBox(nullptr, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    }

	if(curSelType == TXT){

		ew->editorLineHeight = 0;
		int lineHeight = 34;
		HWND temp;

		std::list<void*>::iterator it = s->items.begin();
		std::advance(it, curSel);
		TextSettings * ts = TS((*it));

		//----------------------------------------------------
		//Control name
		//----------------------------------------------------
		int size = SendMessage(hListBox, LB_GETTEXTLEN , curSel, 0);
		char tempName[size+1];
		SendMessage(hListBox, LB_GETTEXT , curSel, (LPARAM)tempName);

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Control name:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_T_NAME, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, tempName);

		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//text to display
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Text to display:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_T_TEXT, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, ts->text.c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//XY
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Horizontal position (X):", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_T_XPOS, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ts->posx).c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Vertical position (Y):", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_T_YPOS, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ts->posy).c_str());
		ew->editorLineHeight += lineHeight;

		//Aligment
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Text alignment:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "Left", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+20, ew->editorLineHeight, 100, 27, righthwnd, (HMENU)INPUT_T_ALIG_0, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "Center", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+100+20, ew->editorLineHeight, 100, 27, righthwnd, (HMENU)INPUT_T_ALIG_1, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "Right", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+200+20, ew->editorLineHeight, 100, 27, righthwnd, (HMENU)INPUT_T_ALIG_2, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetRadioBtn(INPUT_T_ALIG_0, INPUT_T_ALIG_2, INPUT_T_ALIG_0+ts->alignment, righthwnd);
		ratioData.push_back({INPUT_T_ALIG_0,INPUT_T_ALIG_2});
		ew->editorLineHeight += lineHeight;


		//----------------------------------------------------
		//Font choose
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Used font:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "COMBOBOX", "<Click to choose>", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL , 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_T_FONT_P_T, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SendMessage( temp, CB_ADDSTRING, 0,( LPARAM ) "<Select not installed font>" ); //Window popup
		SendMessage( temp, CB_ADDSTRING, 1,( LPARAM ) ts->fontPath.c_str() );

		int comboboxPos = 2;

		_WIN32_FIND_DATAA ffd;
		HANDLE hFind = FindFirstFile(TEXT("C:\\WINDOWS\\Fonts\\*"), &ffd);
		if (INVALID_HANDLE_VALUE != hFind)
		{
		    do
		    {
		    	if(ffd.cFileName[0] != '.'){
		    		std::string checkStr(ffd.cFileName);
		    		if(checkStr.size() > 4 and checkStr[checkStr.size()-1] == 'f' and checkStr[checkStr.size()-2] == 't' and checkStr[checkStr.size()-3] == 't'){
		    			SendMessage( temp, CB_ADDSTRING, comboboxPos,( LPARAM ) ffd.cFileName );
		    			++comboboxPos;
		    		}
		    	}

		    } while(FindNextFile(hFind, &ffd) != 0);
		    FindClose(hFind);
		}


		SendMessage(temp, CB_SETCURSEL, (WPARAM)1, 0 );

		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Font color
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Font color:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "<Click to choose>", WS_CHILD | WS_VISIBLE , 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_T_FONT_C, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, ts->color.c_str());
		ew->editorLineHeight += lineHeight;


		//----------------------------------------------------
		//Font size
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Font size:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_T_FONT_S, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ts->fontSize).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Font opacity
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Text opacity [0-255]:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_T_FONT_O, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ts->opacity).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Info
		//----------------------------------------------------
		std::string info = "Information: \nYou can use variables provided by modules via macros - you just need to type \nit in \"Text to display\" field along with other text data. Available macros:\n\n"
							"{cpu_value}, {ram_value}, {time_minute}, {time_hour}, {time_day_str}, {time_day}, {time_month_str}, {time_month}, {time_year}";

		temp = CreateWindowEx(SS_RIGHT, "STATIC", info.c_str(), WS_CHILD | WS_VISIBLE, 0, ew->editorLineHeight+40, 630,  ew->editorLineHeight*5, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

	} else if(curSelType == GRAPH) {

		ew->editorLineHeight = 0;
		int lineHeight = 34;
		HWND temp;

		std::list<void*>::iterator it = s->items.begin();
		std::advance(it, curSel);
		GraphSettings * gs = GS((*it));

		//----------------------------------------------------
		//Control name
		//----------------------------------------------------
		int size = SendMessage(hListBox, LB_GETTEXTLEN , curSel, 0);
		char tempName[size+1];
		SendMessage(hListBox, LB_GETTEXT , curSel, (LPARAM)tempName);

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Control name:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_NAME, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, tempName);

		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Graph display value
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Display value:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_TEXT, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, gs->text.c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//XY
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Horizontal position (X):", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_XPOS, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(gs->posx).c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Vertical position (Y):", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_YPOS, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(gs->posy).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//WH
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Width [px]:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_WIDTH, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(gs->width).c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Height [px]:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_HEIGHT, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(gs->height).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Graph BASE color
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Base line color:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "<Click to choose>", WS_CHILD | WS_VISIBLE , 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_BASE_C, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, gs->baseColor.c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Bars color
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Single BAR color:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "<Click to choose>", WS_CHILD | WS_VISIBLE , 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_BAR_C, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, gs->barColor.c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Bars width
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Single BAR width:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_BAR_W, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(gs->barWidth).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Bars spacing
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Space between BARS:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_BAR_SPACE, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(gs->barSpace).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Opacity
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Graph opacity:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_G_OPACITY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(gs->opacity).c_str());
		ew->editorLineHeight += lineHeight;


	}else if(curSelType == GSETTINGS){
		ew->editorLineHeight = 0;
		int lineHeight = 34;
		HWND temp;

		//----------------------------------------------------
		//DISPLAY ON MONITOR
		//----------------------------------------------------

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Display on monitor:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_MONITOR_SEL, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(s->gs.monitor).c_str());
		ew->editorLineHeight += lineHeight;


		//----------------------------------------------------
		//GLOBAL DELAY
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Global delay [ms]:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_DELAY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(s->gs.delay).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//THEME BACKGROUND MODE
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Background mode:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "Stretch", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240, ew->editorLineHeight, 75, 27, righthwnd, (HMENU)INPUT_BACKGROUND_MODE_0, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "Oryginal", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+75, ew->editorLineHeight, 80, 27, righthwnd, (HMENU)INPUT_BACKGROUND_MODE_1, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "Fit-In", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+163, ew->editorLineHeight, 60, 27, righthwnd, (HMENU)INPUT_BACKGROUND_MODE_2, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "Fit-Out", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+225, ew->editorLineHeight, 75, 27, righthwnd, (HMENU)INPUT_BACKGROUND_MODE_3, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetRadioBtn(INPUT_BACKGROUND_MODE_0, INPUT_BACKGROUND_MODE_3, INPUT_BACKGROUND_MODE_0+s->gs.backgroundMode, righthwnd);
		ratioData.push_back({INPUT_BACKGROUND_MODE_0,INPUT_BACKGROUND_MODE_3});
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//THEME BACKGROUND OPACITY
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Background opacity:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_BACKGROUND_OPACITY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(s->gs.bgOpacity).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//THEMED BACKGROUND
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Theme backgrounded:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "Yes", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_BACKGROUND_AUTO_1, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "No", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+150+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_BACKGROUND_AUTO_0, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetRadioBtn(INPUT_BACKGROUND_AUTO_0, INPUT_BACKGROUND_AUTO_1, INPUT_BACKGROUND_AUTO_0+s->gs.backgroundAuto, righthwnd);
		ratioData.push_back({INPUT_BACKGROUND_AUTO_0,INPUT_BACKGROUND_AUTO_1});
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//BACKGROUND PATH
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Background path:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );


		temp = CreateWindowEx( 0, "COMBOBOX", "<Click to choose>", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL , 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_BACKGROUND_PATH, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SendMessage( temp, CB_ADDSTRING, 0,( LPARAM ) "<Click to choose>" ); //Window popup
		SendMessage( temp, CB_ADDSTRING, 1,( LPARAM ) s->gs.backgroundPath.c_str() );
		SendMessage(temp, CB_SETCURSEL, 1, 0);


		ew->editorLineHeight += lineHeight;
		ew->editorLineHeight += lineHeight;


		//----------------------------------------------------
		//CPU
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "CPU module state:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "On", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_MODULE_CPU_1, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "Off", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+150+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_MODULE_CPU_0, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetRadioBtn(INPUT_MODULE_CPU_0, INPUT_MODULE_CPU_1, INPUT_MODULE_CPU_0+s->gs.cpu, righthwnd);
		ratioData.push_back({INPUT_MODULE_CPU_0,INPUT_MODULE_CPU_1});
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "CPU module refresh [ms]:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_MODULE_CPU_FPS, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(s->gs.cpuFps).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//RAM
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "RAM module state:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "On", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_MODULE_RAM_1, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "Off", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+150+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_MODULE_RAM_0, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetRadioBtn(INPUT_MODULE_RAM_0, INPUT_MODULE_RAM_1, INPUT_MODULE_RAM_0+s->gs.ram, righthwnd);
		ratioData.push_back({INPUT_MODULE_RAM_0,INPUT_MODULE_RAM_1});
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "RAM module refresh [ms]:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_MODULE_RAM_FPS, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(s->gs.ramFps).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//DISK
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "DISK module state:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "On", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_MODULE_DISK_1, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "Off", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+150+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_MODULE_DISK_0, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetRadioBtn(INPUT_MODULE_DISK_0, INPUT_MODULE_DISK_1, INPUT_MODULE_DISK_0+s->gs.disk, righthwnd);
		ratioData.push_back({INPUT_MODULE_DISK_0,INPUT_MODULE_DISK_1});
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "DISK module refresh [ms]:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_MODULE_DISK_FPS, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(s->gs.diskFps).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//TIME
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "TIME module state:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "On", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_MODULE_TIME_1, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "Off", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+150+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_MODULE_TIME_0, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetRadioBtn(INPUT_MODULE_TIME_0, INPUT_MODULE_TIME_1, INPUT_MODULE_TIME_0+s->gs.time, righthwnd);
		ratioData.push_back({INPUT_MODULE_TIME_0,INPUT_MODULE_TIME_1});
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "TIME module refresh [ms]:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_MODULE_TIME_FPS, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(s->gs.timeFps).c_str());
		ew->editorLineHeight += lineHeight;

		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Load icon manager
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Load icon manager ext:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "On", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_LOAD_EXTENSIONS_1, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		temp = CreateWindowEx( 0, "BUTTON", "Off", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON , 240+150+50, ew->editorLineHeight, 150, 27, righthwnd, (HMENU)INPUT_LOAD_EXTENSIONS_0, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetRadioBtn(INPUT_LOAD_EXTENSIONS_0, INPUT_LOAD_EXTENSIONS_1, INPUT_LOAD_EXTENSIONS_0+s->gs.extOn, righthwnd);
		ratioData.push_back({INPUT_LOAD_EXTENSIONS_0,INPUT_LOAD_EXTENSIONS_1});
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Icon manager hotkey:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_LOAD_EXTENSIONS_HK, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(s->gs.extHK).c_str());
		ew->editorLineHeight += lineHeight;

	} else if(curSelType == LSETTINGS){
		ew->editorLineHeight = 0;
		int lineHeight = 34;
		HWND temp;

		//----------------------------------------------------
		//Days
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Monday as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)MONDAY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.monday.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Tuesday as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)TUESDAY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.tuesday.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Wednesday as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)WEDNESDAY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.wednesday.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Thursday as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)THURSDAY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.thursday.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Friday as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)FRIDAY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.friday.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Saturday as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)SATURDAY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.saturday.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Sunday as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)SUNDAY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.sunday.c_str());
		ew->editorLineHeight += lineHeight;
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Months
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "January as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)JANUARY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.jan.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "February as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)FEBRUARY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.feb.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "March as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)MARCH, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.mar.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "April as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)APRIL, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.apr.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "May as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)MAY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.may.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "June as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)JUNE, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.june.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "July as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)JULY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.july.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "August as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)AUGUST, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.aug.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "September as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)SEPTEMBER, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.sept.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "October as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)OCTOBER, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.oct.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "November as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)NOVEMBER, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.nov.c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "December as:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)DECEMBER, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, s->ls.dec.c_str());
		ew->editorLineHeight += lineHeight;
	}  else if(curSelType == DISK){

		ew->editorLineHeight = 0;
		int lineHeight = 34;
		HWND temp;

		std::list<void*>::iterator it = s->items.begin();
		std::advance(it, curSel);
		DiskSettings * ds = DS((*it));

		//----------------------------------------------------
		//Control name
		//----------------------------------------------------
		int size = SendMessage(hListBox, LB_GETTEXTLEN , curSel, 0);
		char tempName[size+1];
		SendMessage(hListBox, LB_GETTEXT , curSel, (LPARAM)tempName);

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Control name:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_NAME, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, tempName);

		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//text to display
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Disk letter filter:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_TEXT, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, ds->text.c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//XY
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Horizontal position (X):", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_XPOS, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ds->posx).c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Vertical position (Y):", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_YPOS, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ds->posy).c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Horizontal advance (X):", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_ADVX, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ds->posx_inc).c_str());
		ew->editorLineHeight += lineHeight;

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Vertical advance (Y):", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_ADVY, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ds->posy_inc).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Width
		//----------------------------------------------------

		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Disk panel width:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_WIDTH, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ds->width).c_str());
		ew->editorLineHeight += lineHeight;



		//----------------------------------------------------
		//Font choose
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Used font:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "COMBOBOX", "<Click to choose>", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL | WS_VSCROLL , 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_FONT_P, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SendMessage( temp, CB_ADDSTRING, 0,( LPARAM ) "<Select not installed font>" ); //Window popup
		SendMessage( temp, CB_ADDSTRING, 1,( LPARAM ) ds->fontPath.c_str() );

		int comboboxPos = 2;

		_WIN32_FIND_DATAA ffd;
		HANDLE hFind = FindFirstFile(TEXT("C:\\WINDOWS\\Fonts\\*"), &ffd);
		if (INVALID_HANDLE_VALUE != hFind)
		{
		    do
		    {
		    	if(ffd.cFileName[0] != '.'){
		    		SendMessage( temp, CB_ADDSTRING, comboboxPos,( LPARAM ) ffd.cFileName );
		    		++comboboxPos;
		    	}

		    } while(FindNextFile(hFind, &ffd) != 0);
		    FindClose(hFind);
		}


		SendMessage(temp, CB_SETCURSEL, (WPARAM)1, 0 );

		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Font color
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Font color:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "<Click to choose>", WS_CHILD | WS_VISIBLE , 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_FONT_C, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, ds->color.c_str());
		ew->editorLineHeight += lineHeight;


		//----------------------------------------------------
		//Font size
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Font size:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_FONT_S, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ds->fontSize).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Font opacity
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Text opacity [0-255]:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_FONT_O, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ds->opacity).c_str());
		ew->editorLineHeight += lineHeight;


		//------------------------------------------------------------------------------------------
		//NEXT IS DISK PART
		//------------------------------------------------------------------------------------------

		//----------------------------------------------------
		//Border thickness
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Panel thickness:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_BT, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ds->borderThickness).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//Border color 2
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Panel inner color:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "<Click to choose>", WS_CHILD | WS_VISIBLE , 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_FC, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, ds->fillColor.c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//border color 1
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Panel outer color:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( 0, "BUTTON", "<Click to choose>", WS_CHILD | WS_VISIBLE , 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_BC, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, ds->borderColor.c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//panel opacity
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Panel opacity:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_DO, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ds->diskOpacity).c_str());
		ew->editorLineHeight += lineHeight;

		//----------------------------------------------------
		//border height
		//----------------------------------------------------
		temp = CreateWindowEx(SS_RIGHT, "STATIC", "Panel height:", WS_CHILD | WS_VISIBLE | SS_RIGHT, 0, ew->editorLineHeight, 220, 30, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

		temp = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", NULL, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 240, ew->editorLineHeight, 300, 27, righthwnd, (HMENU)INPUT_D_BH, hInst, NULL );
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		SetWindowText(temp, std::to_string(ds->borderHeight).c_str());
		ew->editorLineHeight += lineHeight;


		//----------------------------------------------------
		//Info
		//----------------------------------------------------
		std::string info = "Information: \nYou can use variables provided by \"DISK\" module via macros - you just need to type it in \"Disk letter filter\" field. Available macros:\n\n"
							"{exclude}, {<Drive Letter>} -> {C}, {D} ... \n\nIf you leave that field empty - all drives will be displayed. If you just type Drive Letter macros - only selected drives will be displayed. If you use {exclude} macro following by drive letter macos - all drives will be displayed excluding letters provided into field.";

		temp = CreateWindowEx(SS_RIGHT, "STATIC", info.c_str(), WS_CHILD | WS_VISIBLE, 0, ew->editorLineHeight+40+20, 630,  ew->editorLineHeight, righthwnd, (HMENU)1000, hInst, NULL);
		SendMessage(temp, WM_SETFONT, (WPARAM)rh->fontCnt.at(FONT_SMALL), TRUE);
		InvalidateRect( temp, NULL, TRUE );

	}

	int fix = 0;
	if(curSelType == DISK){
		ew->fixTip = 5;
		fix =  34*9;
	}

	RECT windowRect;
	GetClientRect( righthwnd, & windowRect );
	SCROLLINFO si;
	ZeroMemory( & si, sizeof( si ) );
	si.cbSize = sizeof( SCROLLINFO );
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = ew->editorLineHeight+fix;
	si.nPage = 545;
	si.nPos = 0;
	SetScrollInfo( righthwnd, SB_VERT, & si, TRUE );

	//InvalidateRect(righthwnd, NULL, TRUE );
	UpdateWindow(righthwnd);

}

void EditorWindow::SetRadioBtn(int resFirst, int resSec, int toSet, HWND hDlg){
	for(int j = 0; j<(resSec-resFirst)+1; j++){
		SendDlgItemMessage(hDlg, resFirst+j, BM_SETCHECK, 0, 0);
	}
	SendDlgItemMessage(hDlg, toSet, BM_SETCHECK, 1, 0);
}


void EditorWindow::setCurSel(int toSel){
	HBITMAP hb = (HBITMAP)SendMessage(hListBox, LB_GETITEMDATA, toSel, 0);
	if(hb == (HBITMAP)rh->resCnt.at(GSETTINGS_BITMAP)){
		std::cout<<"General"<<std::endl;
		curSelType = GSETTINGS;
	} else if(hb == (HBITMAP)rh->resCnt.at(TXT_BITMAP)){
		std::cout<<"TXT"<<std::endl;
		curSelType = TXT;
	} else if(hb == (HBITMAP)rh->resCnt.at(GRAPH_BITMAP)){
		std::cout<<"Graph"<<std::endl;
		curSelType = GRAPH;
	} else if(hb == rh->resCnt.at(LSETTINGS_BITMAP)){
		std::cout<<"Language"<<std::endl;
		curSelType = LSETTINGS;
	} else if(hb == rh->resCnt.at(DISK_BITMAP)){
		std::cout<<"Disk"<<std::endl;
		curSelType = DISK;
	}
	curSel = toSel;
}

