#pragma once
#include <string>

namespace settings {
	extern std::string save_path;

	void save();
	void load();
}