#include <vector>
#include <string>
#include "screenshot_handler.h"
#include "../../controller/take_screenshot_controller/take_screenshot_controller.h"
#include "../image/image.h"
#include "../settings/settings.h"

std::vector<POINT> selected_points = {};

bool screenshot_handler::is_screenshot_in_progress = false;

// https://stackoverflow.com/questions/9524393/how-to-capture-part-of-the-screen-and-save-it-to-a-bmp
bool screenshot(int x, int y, int width, int height, const std::string& path) {
    HDC hdc_source = GetDC(nullptr);
    HDC hdc_memory = CreateCompatibleDC(hdc_source);

    HBITMAP bmp = CreateCompatibleBitmap(hdc_source, width, height);
    HBITMAP bmp_old = static_cast<HBITMAP>(SelectObject(hdc_memory, bmp));

    BitBlt(hdc_memory, 0, 0, width, height, hdc_source, x, y, SRCCOPY);
    bmp = static_cast<HBITMAP>(SelectObject(hdc_memory, bmp_old));

    DeleteDC(hdc_source);
    DeleteDC(hdc_memory);

    HPALETTE pallete = nullptr;
    bool result = false;

    std::tuple<unsigned char*, long, IStream*> bitmap_bytes_res;
    if (!image::get_bitmap_bytes(bmp, pallete, &bitmap_bytes_res)) {
        return false;
    }

    return image::convert_bmp_to_png(path, bitmap_bytes_res);
}

LRESULT __stdcall screenshot_handler::window_proc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {
    switch (msg) {
        case WM_LBUTTONUP: {
            POINT cursor_position;
            GetCursorPos(&cursor_position);

            if (selected_points.size() < 1) {
                // Started the screenshot process.
                selected_points.insert(selected_points.begin(), cursor_position);
                return 0;
            } else {
                // Ending the screenshot process.
                selected_points.insert(selected_points.begin() + 1, cursor_position);
            }

            int start_x = selected_points.at(0).x;
            int start_y = selected_points.at(0).y;

            int end_x = selected_points.at(1).x;
            int end_y = selected_points.at(1).y;

            if (end_x < start_x) {
                int old = end_x;
                end_x = start_x;
                start_x = old;
            }

            if (end_y < start_y) {
                int old = end_y;
                end_y = start_y;
                start_y = old;
            }

            screenshot(start_x, start_y, end_x - start_x, end_y - start_y, settings::save_path + "\\image.png");

            selected_points.clear();

            take_screenshot_controller::took_screenshot();

            screenshot_handler::is_screenshot_in_progress = false;

            return 0;
        }
    }

    return DefWindowProc(hwnd, msg, w_param, l_param);
}