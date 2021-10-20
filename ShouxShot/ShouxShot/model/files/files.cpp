#include <Windows.h>
#include <fstream>
#include <filesystem>
#include "files.h"
#include "../utility/utility.h"

std::string files::get_executable_path() { // Location of the executable file. NOT the current working directory.
    char path[MAX_PATH];
    std::string string_path = std::string(path, GetModuleFileNameA(nullptr, path, sizeof(path)));
    return string_path + "\\..";
}

void files::setup_shouxshot_files() {
	std::string save_path = get_executable_path() + "\\images";
	std::string settings_path = get_executable_path() + "\\settings.json";

	if (!std::filesystem::exists(save_path)) {
		std::filesystem::create_directory(save_path);
	}

	if (!std::filesystem::exists(settings_path)) {
		utility::replace(save_path, "\\", "/");
		files::save_file(settings_path, "{\"save_path\":\"" + save_path + "\"}");
	}
}

void files::save_file(const std::string& path, const std::string& data) {
	std::ofstream out_file;
	out_file.open(path);
	out_file << data;
	out_file.close();
}

void files::load_file(const std::string& path, std::string& data) {
	std::ifstream in_file;
	in_file.open(path);
	in_file >> data;
	in_file.close();
}