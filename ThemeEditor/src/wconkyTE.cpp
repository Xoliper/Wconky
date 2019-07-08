#include <windows.h>
#include <winuser.h>
#include <commctrl.h>
#include <iostream>
#include <Strsafe.h>

#include "Resources.hpp"
#include "ResHolder.hpp"
#include "ControlsTheme.hpp"

#include "ListWindow.hpp"
#include "EditorWindow.hpp"
#include "Settings.hpp"

#include "Libs/XmlParser.hpp"

ListWindow * lw = NULL;
EditorWindow * ew = NULL;
ResHolder * rh = NULL;
Settings * s = NULL;

void RegisterWindowClass(HINSTANCE hInst, const char arr[], int colorOff,  WNDPROC func){

	WNDCLASSEX wc;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = func;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = sizeof (long); //0
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+colorOff);
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = arr;
    wc.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);

    if(!RegisterClassEx(&wc)){
         MessageBox(nullptr, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
         exit(0);
     } else {
    	 if(DEBUG) std::cout<<"Window class: '"<<arr<<"' registered!"<<std::endl;
     }
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    PAINTSTRUCT ps;
    HDC hDC;

    switch(msg)
    {

    case WM_PAINT: {
        hDC = BeginPaint(hwnd, &ps);
        RECT rc;
        GetClientRect( hwnd, &rc );
        rc.left = 10;
    	rc.right -= 10;
    	rc.top = 60;
    	rc.bottom = 62;
		if(rh != NULL) FillRect(hDC, &rc, rh->brushCnt.find(BRUSH_BLUE)->second);
    	rc.top = 120;
    	rc.bottom = 122;
		EndPaint(hwnd, &ps);

    } return 0L;


    case WM_CTLCOLORSTATIC:
    	SetBkMode((HDC)wParam,TRANSPARENT);
    	SetTextColor((HDC)wParam, RGB(110,110,110));
    return (INT_PTR)(COLOR_WINDOW+1);

    case WM_DRAWITEM: {
    	DRAWITEMSTRUCT *dis = (DRAWITEMSTRUCT*)lParam;
    	if(dis->CtlType == ODT_BUTTON){
    		DrawHeaderBtn(dis, dis->hwndItem);
    	}
    } return TRUE;

    case WM_SIZE: {
        InvalidateRect(hwnd, NULL, TRUE);
    } break;


    case WM_CLOSE:
    	DestroyWindow(hwnd);
    break;

    case WM_DESTROY:
    	PostQuitMessage(0);
    break;

    default:
    	return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    //Create Window
    RegisterWindowClass(hInstance, "WConky Manager", 1, MainWndProc);
    RegisterWindowClass(hInstance, "Wconky Theme Editor", 1, WndProc);
    RegisterWindowClass(hInstance, "Edit Window Item", 1, WndProc2);


    HWND hwnd = NULL;
    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "Wconky Theme Editor", "Wconky Theme Editor", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 720, nullptr, nullptr, hInstance, nullptr);
    if(hwnd == nullptr){
        MessageBox(nullptr, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        exit(0);
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //Load Resources
    rh = new ResHolder(hwnd, hInstance);
    InvalidateRect(hwnd, NULL, TRUE);

	//Prepare sub-window objs
	ew = new EditorWindow(hInstance, hwnd);
	lw = NULL;

	s = new Settings();

	XmlParser xp;
	if(xp.load("config.xml", true)){
		xp.parse();
		s->LoadGSettings(&xp);
		s->LoadLSettings(&xp);
		ew->AddItem("General Settings", rh->resCnt.at(GSETTINGS_BITMAP));
	    ew->AddItem("Language Settings", rh->resCnt.at(LSETTINGS_BITMAP));
		s->LoadItems(&xp, ew);
	} else {
		exit(0);
	}

    ew->SelectFirst();


    //Main Loop
    MSG Msg;
    while(GetMessage(&Msg, nullptr, 0, 0) > 0) //przerobic na peek message
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    delete rh;

    return Msg.wParam;
}
