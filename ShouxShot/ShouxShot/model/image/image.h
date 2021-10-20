#pragma once
#include <Windows.h>
#include <string>

namespace image {
	bool get_bitmap_bytes(HBITMAP bmp, HPALETTE pallete, std::tuple<unsigned char*, long, IStream*>* bitmap_bytes);
	bool convert_bmp_to_png(const std::string& path, std::tuple<unsigned char*, long, IStream*> bitmap_bytes);
};