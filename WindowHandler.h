#pragma once
#include <cstdlib>
#include <algorithm>
#include <vector>
#include "WindowClass.h"

class MapEditer : public Window::EventHandler {
//public:
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {

		switch (msg)
		{
		case WM_CREATE: {
			HTile = (HBITMAP)LoadImage(GetModuleHandle(nullptr), FName2, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
			BITMAP B = { 0, };
			GetObject(HTile, sizeof(BITMAP), &B);
			Child.W = B.bmWidth;
			Child.H = B.bmHeight;
			Child.X = 64;
			Child.Y = 64;
			WMAX = 640 / TileX;
			HMAX = 640 / TileY;
			Tiles.resize(WMAX*HMAX);
			break;
		}
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
				Tiles[Pos] = ST;
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
		return 0;
	

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
	TCHAR FName[32] = _T("Tile.bmp");
	TCHAR FName2[32] = _T("Tile2.bmp");
	R<int> Child = {0,0,128,128};
	UINT TileY = 32;//Pixel
	UINT TileX = 32;//Pixel
	UINT WMAX = 128;
	UINT HMAX = 128;
	std::vector<std::uint16_t> Tiles;
	UINT ST = 0;
};