#include <iostream>
#include <Windows.h>
#include "take_screenshot_controller.h"
#include "../../model/input/input.h"
#include "../../model/monitor/monitor.h"
#include "../../model/screenshot_handler/screenshot_handler.h"
#include "../../view/window/window.h"

HWND screenshot_handler_hwnd;

void take_screenshot_controller::activate() {
    if (take_screenshot_controller::is_screenshot_in_progress()) return;

    screenshot_handler::is_screenshot_in_progress = true;

    POINT most_top_position = monitor::get_most_top_position();
    POINT all_monitors_size = monitor::get_full_size();

    screenshot_handler_hwnd = window::initiate(
        L"ScreenshotHandler",
        most_top_position.x, most_top_position.y,
        all_monitors_size.x, all_monitors_size.y,
        WS_EX_LAYERED /*| WS_EX_TRANSPARENT*/ | WS_EX_TOPMOST,
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,
        screenshot_handler::window_proc, GetModuleHandleA(nullptr)
    );

    SetWindowLong(screenshot_handler_hwnd, GWL_STYLE, 0);
    ShowWindow(screenshot_handler_hwnd, SW_SHOW);
}

void take_screenshot_controller::took_screenshot() {
    DestroyWindow(screenshot_handler_hwnd);
}

bool take_screenshot_controller::is_screenshot_in_progress() {
    return screenshot_handler::is_screenshot_in_progress;
}