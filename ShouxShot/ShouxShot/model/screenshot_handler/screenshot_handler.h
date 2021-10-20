#pragma once
#include <Windows.h>

namespace screenshot_handler {
	LRESULT WINAPI window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param);

	extern bool is_screenshot_in_progress;
};