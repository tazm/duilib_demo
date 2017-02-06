#include <Windows.h>
#include <tchar.h>
#include <Shlwapi.h>

#include "MainWnd.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    CPaintManagerUI::SetInstance(hInstance);
    TCHAR tszSkinPath[MAX_PATH] = {0};

    GetModuleFileNameW(NULL, tszSkinPath, MAX_PATH);
    PathAppendW(tszSkinPath, L"\\..");
    PathAddBackslashW(tszSkinPath);
    PathAppendW(tszSkinPath, TEXT("skin"));

    CPaintManagerUI::SetResourcePath(tszSkinPath);

#ifndef _DEBUG
    CPaintManagerUI::SetResourceZip(_T("tree.zip"));
#endif

    MainWnd* pWnd = new MainWnd();
    pWnd->Create(NULL, L"tree", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    pWnd->ShowWindow();

    CPaintManagerUI::MessageLoop();

    return 0;
}