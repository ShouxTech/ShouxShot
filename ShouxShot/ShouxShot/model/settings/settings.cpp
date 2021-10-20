#include "settings.h"
#include "../files/files.h"
#include "../json/json.hpp"
#include "../utility/utility.h"

std::string settings::save_path = "";

using json = nlohmann::json;

template<typename T>
void load_setting(T& setting, const json& json_object, const std::string& setting_name) {
	if (!json_object.contains(setting_name)) return;
	setting = json_object[setting_name];
}

void settings::save() {
	utility::replace(settings::save_path, "\\", "/");

	json data;
	data["save_path"] = settings::save_path;

	files::save_file(files::get_executable_path() + "\\settings.json", data.dump());
}

void settings::load() {
	std::string data_string = "";
	files::load_file(files::get_executable_path() + "\\settings.json", data_string);

	json data = json::parse(data_string);
	load_setting<std::string>(settings::save_path, data, "save_path");
}