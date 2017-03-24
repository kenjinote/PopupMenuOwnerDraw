#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "resource.h"

TCHAR szClassName[] = TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMenu;
	switch (msg)
	{
	case WM_CREATE:
		hMenu = LoadMenu(GetModuleHandle(0), (LPCTSTR)IDR_MENU1);
		break;
	case WM_MEASUREITEM:
		{
			((LPMEASUREITEMSTRUCT)lParam)->itemWidth = 0;
			((LPMEASUREITEMSTRUCT)lParam)->itemHeight = 20;
		}
		return TRUE;
	case WM_DRAWITEM:
		switch (((LPDRAWITEMSTRUCT)lParam)->itemID)
		{
		case ID_POPUP_CCC:
			if (((LPDRAWITEMSTRUCT)lParam)->itemState&ODS_DISABLED)
			{
				FillRect(((LPDRAWITEMSTRUCT)lParam)->hDC, &((LPDRAWITEMSTRUCT)lParam)->rcItem, (HBRUSH)(COLOR_MENU + 1));
				SetBkMode(((LPDRAWITEMSTRUCT)lParam)->hDC, TRANSPARENT);
				SetTextColor(((LPDRAWITEMSTRUCT)lParam)->hDC, GetSysColor(COLOR_GRAYTEXT));

			}
			else if (((LPDRAWITEMSTRUCT)lParam)->itemState&ODS_SELECTED)
			{
				FillRect(((LPDRAWITEMSTRUCT)lParam)->hDC, &((LPDRAWITEMSTRUCT)lParam)->rcItem, (HBRUSH)(COLOR_HIGHLIGHT + 1));
				SetBkMode(((LPDRAWITEMSTRUCT)lParam)->hDC, TRANSPARENT);
				SetTextColor(((LPDRAWITEMSTRUCT)lParam)->hDC, RGB(255, 255, 255));
			}
			else
			{
				FillRect(((LPDRAWITEMSTRUCT)lParam)->hDC, &((LPDRAWITEMSTRUCT)lParam)->rcItem, (HBRUSH)(COLOR_MENU + 1));
				SetBkMode(((LPDRAWITEMSTRUCT)lParam)->hDC, TRANSPARENT);
				SetTextColor(((LPDRAWITEMSTRUCT)lParam)->hDC, RGB(0, 0, 0));
			}
			RECT rect = ((LPDRAWITEMSTRUCT)lParam)->rcItem;
			rect.left += 18;
			DrawText(((LPDRAWITEMSTRUCT)lParam)->hDC, (LPCTSTR)(((LPDRAWITEMSTRUCT)lParam)->itemData), -1, &rect, DT_SINGLELINE | DT_VCENTER);
			return FALSE;
		}
		break;
	case WM_RBUTTONDOWN:
		{
			POINT point = { LOWORD(lParam), HIWORD(lParam) };
			ClientToScreen(hWnd, &point);
			HMENU hSubMenu = GetSubMenu(hMenu, 0);
			TCHAR szMenuText[256];
			GetMenuString(hSubMenu, ID_POPUP_CCC, szMenuText, _countof(szMenuText), MF_BYCOMMAND);
			ModifyMenu(hSubMenu, ID_POPUP_CCC, MF_BYCOMMAND | MF_OWNERDRAW | MF_DISABLED, ID_POPUP_CCC, szMenuText);
			TrackPopupMenu(hSubMenu, TPM_LEFTALIGN, point.x, point.y, 0, hWnd, NULL);
		}
		break;
	case WM_DESTROY:
		DestroyMenu(hMenu);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("ポップアップメニューのオーナードロー"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}
