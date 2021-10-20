#pragma once

namespace files {
	std::string get_executable_path(); // Location of the executable file. NOT the current working directory.

	void setup_shouxshot_files();

	void save_file(const std::string& path, const std::string& data);
	void load_file(const std::string& path, std::string& data);
};