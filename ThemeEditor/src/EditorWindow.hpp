#ifndef EDITORWINDOW_HPP_
#define EDITORWINDOW_HPP_

#include <windows.h>
#include <winuser.h>
#include <commctrl.h>
#include <iostream>
#include <Strsafe.h>
#include <vector>
#include "Resources.hpp"
#include "ResHolder.hpp"
#include "ControlsTheme.hpp"
#include "Settings.hpp"

static COLORREF Colors[ 16 ];

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc2(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct {
	int start;
	int end;
} Range;

class EditorWindow {
public:
	EditorWindow(HINSTANCE hInst, HWND hwnd);
	virtual ~EditorWindow();

	void AddItem(char pstr [], HBITMAP hbmp, bool mode = false);
	void SelectFirst();
	void setCurSel(int toSel);
	void LoadForm(int sel);
	void SetRadioBtn(int resFirst, int resSec, int toSet, HWND hDlg);
	void SaveItemData();


	int helper = -1;

	HWND button;
	HWND saveButton;
	HWND addTxtBtn;
	HWND addGraphBtn;
	HWND addDiskBtn;
	HWND deleteBtn;
	HWND hListBox;
	HWND righthwnd;

	int curSel;
	int curSelType;
	int editorLineHeight = 0;

	int fixTip = 0;

	std::vector<Range> ratioData;

private:
	HWND ohwnd;
	HINSTANCE hInst;

};

extern ResHolder * rh;
extern EditorWindow * ew;
extern Settings * s;

#endif /* EDITORWINDOW_HPP_ */
