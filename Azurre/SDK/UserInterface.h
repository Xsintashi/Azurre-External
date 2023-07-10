#pragma once

#include <string>
#include "Interfaces.h"
#include "../Core.h"

inline void clientCmd(const std::string& cmd) noexcept {
    COPYDATASTRUCT message{};
    message.dwData = 0;
    message.lpData = (void*)(cmd.c_str());
    message.cbData = cmd.size() + 1;
    SendMessageA(IConsole, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&message));
}

inline void clientCmd(const std::u8string& cmd) noexcept {
    COPYDATASTRUCT message{};
    message.dwData = 0;
    message.lpData = (void*)(cmd.c_str());
    message.cbData = cmd.size() + 1;
    SendMessageA(IConsole, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&message));
}
