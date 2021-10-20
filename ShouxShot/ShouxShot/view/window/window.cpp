#include "window.h"
#include "../user_interface/user_interface.h"

HWND window::initiate(const std::wstring& name, int x, int y, int width, int height, DWORD ex_style, DWORD style, WNDPROC window_proc, HINSTANCE hinstance) {
    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        window_proc,
        0L,
        0L,
        hinstance,
        NULL,
        NULL,
        NULL,
        NULL,
        (name + L"Class").c_str(),
        NULL
    };
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowExW(ex_style, wc.lpszClassName, name.c_str(), style, x, y, width, height, NULL, NULL, wc.hInstance, NULL);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    return hwnd;
}