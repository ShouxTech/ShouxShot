#pragma once
#include <Windows.h>
#include <string>

namespace window {
	HWND initiate(const std::wstring& name, int x, int y, int width, int height, DWORD ex_style, DWORD style, WNDPROC window_proc, HINSTANCE hinstance);
};