#pragma once

#include <string>

namespace usr0 {
	void SendConsoleCommand(const std::string& cmd);
	void SendConsoleCommand(const std::u8string& cmd);
}