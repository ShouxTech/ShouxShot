#include "monitor.h"

POINT top_most_position = { 9e9, 9e9 };
POINT full_size = {};

BOOL monitor_enum_proc(HMONITOR monitor, HDC hdc, RECT* rect, LPARAM unnamed_param4) {
	MONITORINFO monitor_info = {};
	monitor_info.cbSize = sizeof(MONITORINFO);
	GetMonitorInfoA(monitor, &monitor_info);

	RECT monitor_rect = monitor_info.rcMonitor;

	if (monitor_rect.left < top_most_position.x) {
		top_most_position.x = monitor_rect.left;
	}
	if (monitor_rect.top < top_most_position.y) {
		top_most_position.y = monitor_rect.top;
	}

	full_size.x += monitor_rect.right - monitor_rect.left;
	full_size.y += monitor_rect.bottom - monitor_rect.top;

	return TRUE;
}

POINT monitor::get_most_top_position() {
	top_most_position = { static_cast<long>(9e9), static_cast<long>(9e9) };
	EnumDisplayMonitors(nullptr, nullptr, monitor_enum_proc, NULL);
	return top_most_position;
}

POINT monitor::get_full_size() {
	full_size = { 0, 0 };
	EnumDisplayMonitors(nullptr, nullptr, monitor_enum_proc, NULL);
	return full_size;
}