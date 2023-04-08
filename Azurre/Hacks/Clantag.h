#pragma once
namespace Clan {
    void setClanTag(const char* name, const char* team);
    void update(bool reset = false, bool update = false) noexcept;
};