#pragma once
#include <functional>

namespace input {
	void begin_listening();

	std::tuple<int*, bool, std::function<void()>> input_began(int* input_key, std::function<void()> callback);

	void remove_connection(int* input_key);
};