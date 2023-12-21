#pragma once
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "WindowClass.h"
/** /
	INT_PTR CALLBACK DP(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
		switch (msg) {
		case WM_CLOSE:
			EndDialog(hwnd, IDOK);
			//DLGBOX = NULL;
			return TRUE;
		}
		return FALSE;
	}
	/**/
class MapEditer : public Window::EventHandler {

//public:
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
		int TextH = 24;
		int TextW = 48;
		switch (msg)
		{
		case WM_CREATE: {

			OPENFILENAME ofn;       // common dialog box structure
			TCHAR szFile[260] = { 0 };       // if using TCHAR macros

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = _T("All\0*.*\0BITMAP\0*.bmp\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&ofn) == TRUE)
			{
				HTile = (HBITMAP)LoadImage(GetModuleHandle(nullptr), ofn.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
				// use ofn.lpstrFile
			}
			else {
				PostQuitMessage(WM_QUIT);
				return 0;
			}
/** /
			DialogBox(
				(HINSTANCE)GetModuleHandle(nullptr),
				TEXT("KITTY"), hWnd, DP
		);
/**/
			E1 = CreateWindow(
				TEXT("EDIT"), TEXT("32"),
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				0, 0, TextW, TextH, hWnd, (HMENU)1,
				((LPCREATESTRUCT)(lp))->hInstance, NULL
			);
			E2 = CreateWindow(
				TEXT("EDIT"), TEXT("32"),
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				TextW, 0, TextW, TextH, hWnd, (HMENU)2,
				((LPCREATESTRUCT)(lp))->hInstance, NULL
			);
			B1 = CreateWindow(
				TEXT("BUTTON"), TEXT("MapSize"),
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				TextW*2, 0, TextW*2, TextH,
				hWnd,(HMENU) 3,((LPCREATESTRUCT)(lp))->hInstance, NULL
			);
			E3 = CreateWindow(
				TEXT("EDIT"), TEXT("32"),
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				TextW*4, 0, TextW, TextH, hWnd, (HMENU)4,
				((LPCREATESTRUCT)(lp))->hInstance, NULL
			);
			E4 = CreateWindow(
				TEXT("EDIT"), TEXT("32"),
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				TextW*5, 0, TextW, TextH, hWnd, (HMENU)5,
				((LPCREATESTRUCT)(lp))->hInstance, NULL
			);
			B2 = CreateWindow(
				TEXT("BUTTON"), TEXT("TileSize"),
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				TextW*6, 0, TextW*2, TextH,
				hWnd,(HMENU) 6,((LPCREATESTRUCT)(lp))->hInstance, NULL
			);
			BITMAP B = { 0, };
			GetObject(HTile, sizeof(BITMAP), &B);
			Child.W = B.bmWidth;
			Child.H = B.bmHeight;
			Child.X = 64;
			Child.Y = 64;
			WMAX = 8; // 640 / TileX;
			HMAX = 8; // 640 / TileY;
			Tiles.resize(WMAX*HMAX);
			break;
		}
		case WM_KEYDOWN:
			if (wp == VK_SPACE) {
				BOOL F = IsWindowVisible(E1);
				if (F == TRUE) {
					ShowWindow(E1,SW_HIDE);
					ShowWindow(E2,SW_HIDE);
					ShowWindow(B1,SW_HIDE);
					ShowWindow(E3,SW_HIDE);
					ShowWindow(E4,SW_HIDE);
					ShowWindow(B2,SW_HIDE);
					F = FALSE;
				}
				else {
					ShowWindow(E1,SW_SHOW);
					ShowWindow(E2,SW_SHOW);
					ShowWindow(B1,SW_SHOW);
					ShowWindow(E3,SW_SHOW);
					ShowWindow(E4,SW_SHOW);
					ShowWindow(B2,SW_SHOW);
					F = TRUE;
				}
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wp))	{
			case 3:
				if (HIWORD(wp) == BN_CLICKED) {
					CHAR X[1024] = { 0, };
					UINT V = 0;
					GetWindowTextA(E1, X, 1024);;
					WMAX = atoi(X);
					GetWindowTextA(E2, X, 1024);;
					HMAX= atoi(X);
					Tiles.resize(0);
					Tiles.resize(WMAX*HMAX);
					InvalidateRect(hWnd, nullptr, TRUE);
				}
				break;
			case 6:
				if (HIWORD(wp) == BN_CLICKED) {
					CHAR X[1024] = { 0, };
					UINT V = 0;
					GetWindowTextA(E3, X, 1024);;
					TileX = atoi(X);
					GetWindowTextA(E4, X, 1024);;
					TileY= atoi(X);

					InvalidateRect(hWnd, nullptr, TRUE);
				}
				break;
			default:
				break;
			}
			return 0;
		case WM_RBUTTONDOWN: {
			UINT X = GET_X_LPARAM(lp);
			UINT Y = GET_Y_LPARAM(lp);
			if (InR(X, Y)) { InvalidateRect(hWnd, nullptr, TRUE); break; }
			Child.X = X;
			Child.Y = Y;
			InvalidateRect(hWnd, nullptr, TRUE);
			break;
		}
		case WM_LBUTTONDOWN: {
			UINT X = GET_X_LPARAM(lp);
			UINT Y = GET_Y_LPARAM(lp);
			if (InR(X, Y)) {
				UINT XX = X - Child.X;
				UINT YY = Y - Child.Y;
				ST = WMAX * (YY / TileY) + (XX / TileX);
			}
			else {
				int Pos = (Y / TileY) * WMAX + (X / TileX);
				if (Pos < Tiles.size()) {
					Tiles[Pos] = ST;
				}
			}
			InvalidateRect(hWnd, nullptr, TRUE);
			break;

		}
		case WM_PAINT:
		{
			HBRUSH HB = CreateSolidBrush(RGB(0x0,0x0,0x0));

			PAINTSTRUCT PS = { 0, };
			HDC h = BeginPaint(hWnd, &PS);
			HGDIOBJ R = SelectObject(h, HB);
			HDC hc = CreateCompatibleDC(h);
			HGDIOBJ Rc = SelectObject(hc, HTile);

			for (int i = 0; i < HMAX; i++) {
				for (int j = 0; j < WMAX; j++) {
					int S = Tiles[i * WMAX + j];
					auto F = BitBlt(h, TileX * j, TileY * i, TileX, TileY, hc, (S % WMAX) * TileX, (S / HMAX) * TileY, SRCCOPY);
				}
			}
			Rectangle(h, Child.X-1, Child.Y-1, Child.X + Child.W+1,Child.Y+ Child.H+1);
			BitBlt(h, Child.X, Child.Y, Child.W, Child.H, hc, 0, 0, SRCCOPY);

			EndPaint(hWnd, &PS);
			
			SelectObject(h, HB);
			DeleteObject(HB);
			SelectObject(hc, Rc);
			DeleteDC(hc);
			
			break;
		}
		case WM_DESTROY:
			DeleteObject(HTile);
			HTile = nullptr;
			PostQuitMessage(WM_QUIT);
			break;


		default:
			return DefWindowProc(hWnd, msg, wp, lp);
			break;
		}
		return 1;
	

	}
protected:

	template<class T>
	struct R{
		T X{};
		T Y{};
		T W{};
		T H{};
	};

	bool InR(int X,int Y) {
		if (Child.X > X)return false;
		if (Child.Y > Y)return false;
		if (Child.X+Child.W < X)return false;
		if (Child.Y+Child.H < Y)return false;

		return true;
	}
protected:
	HBITMAP HTile = nullptr;
//	TCHAR FName[32] = _T("Tile.bmp");
//	TCHAR FName2[32] = _T("Tile2.bmp");
	R<int> Child = {0,0,128,128};
	UINT TileY = 32;//Pixel
	UINT TileX = 32;//Pixel
	UINT WMAX = 128;
	UINT HMAX = 128;
	std::vector<std::uint16_t> Tiles;
	UINT ST = 0;
//	HWND hdlg = nullptr;
	HWND E1 = nullptr;
	HWND E2 = nullptr;
	HWND B1 = nullptr;
	HWND E3 = nullptr;
	HWND E4 = nullptr;
	HWND B2 = nullptr;
};