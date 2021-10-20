#include <iostream>
#include <Windows.h>
#include <thread>
#include "../view/window/window.h"
#include "../view/user_interface/user_interface.h"
#include "../model/files/files.h"
#include "../model/settings/settings.h"
#include "../model/input/input.h"

int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE hinst_prev, PSTR cmdline, int cmdshow) {
#ifndef NDEBUG // Compiling in debug mode will result in a console being created.
    AllocConsole();
    FILE* f = nullptr;
    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
#endif

    files::setup_shouxshot_files();
    settings::load();

    std::atexit(settings::save);

    HWND hwnd;
    std::thread interface_thread = std::thread([&] {
        hwnd = window::initiate(L"ShouxShot", 100, 100, 500, 300, 0L, WS_OVERLAPPEDWINDOW, user_interface::window_proc, hinstance);

        RECT window_rect;
        GetWindowRect(hwnd, &window_rect);
        user_interface::width = window_rect.right - window_rect.left;
        user_interface::height = window_rect.bottom - window_rect.top;

        user_interface::initiate(hwnd); // This is blocking.
    });

    input::begin_listening(); // This is blocking.
}