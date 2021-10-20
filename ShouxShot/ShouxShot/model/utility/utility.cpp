#include "utility.h"

void utility::replace(std::string& str, const std::string& from, const std::string& to) {
	while (true) {
		std::size_t startPosition = str.find(from);
		if (startPosition == std::string::npos) return;
		str.replace(startPosition, from.length(), to);
	}
}