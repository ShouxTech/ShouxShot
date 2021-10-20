#include <Windows.h>
#include <vector>
#include "input.h"

std::vector<std::tuple<int*, bool, std::function<void()>>> connections;

void input::begin_listening() {
	while (true) {
		for (auto& connection : connections) {
			bool old_is_pressed = std::get<1>(connection);

			int key = *std::get<0>(connection);
			if (key == -1) continue;

			if (GetAsyncKeyState(key)) {
				if (!old_is_pressed) {
					std::get<2>(connection)();
					std::get<1>(connection) = true;
				}
			} else {
				std::get<1>(connection) = false;
			}
		}

		Sleep(50);
	}
}

std::tuple<int*, bool, std::function<void()>> input::input_began(int* input_key, std::function<void()> callback) {
	auto tuple = std::make_tuple(input_key, false, callback);
	connections.push_back(tuple);
	return tuple;
}

void input::remove_connection(int* input_key) {
	for (int i = 0; i < connections.size(); i++) {
		auto connection = connections.at(i);
		if (std::get<0>(connection) == input_key) {
			connections.erase(connections.begin() + i);
			break;
		}
	}
}