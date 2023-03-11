#include "UserInterface.h"
#include "Interfaces.h"
#include "../Core.h"

void usr0::SendConsoleCommand(const std::string& cmd)
{
    COPYDATASTRUCT message{};
    message.dwData = 0;
    message.lpData = (void*)(cmd.c_str());
    message.cbData = cmd.size() + 1;
    SendMessageA(IConsole, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&message));
}

void usr0::SendConsoleCommand(const std::u8string& cmd)
{
    COPYDATASTRUCT message{};
    message.dwData = 0;
    message.lpData = (void*)(cmd.c_str());
    message.cbData = cmd.size() + 1;
    SendMessageA(IConsole, WM_COPYDATA, 0, reinterpret_cast<LPARAM>(&message));
}