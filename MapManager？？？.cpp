#include <Windows.h>

#include "WindowClass.h"
#include "WindowHandler.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {

    Window W;
    Window::SharedEventHandler WE = std::make_shared<MapEditer>();
    W.SetEventHandler(WE);

    TCHAR CLASSNAME[] = _T("MapEditer.");
    TCHAR APPNAME[] = _T("TIler!!");

    if (!W.Create(hInstance, CLASSNAME, APPNAME, 640, 640)) { return -1; }
    W.ShowWindow(nCmdShow);
    W.UpdateWindow();

    return W.GetMessageLoop(0, 0);

}