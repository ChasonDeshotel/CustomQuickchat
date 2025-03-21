#include "EngineValidator.h"
#include "GameDefines.hpp"
#include "SdkHeaders.hpp"
#include <Psapi.h>

#include <string>
TArray<UObject*>* GObjects;
TArray<FNameEntry*>* GNames;

bool
EngineValidator::init() {
    GObjects = reinterpret_cast<TArray<UObject*>*>(GetGObjectsAddress());
    GNames = reinterpret_cast<TArray<FNameEntry*>*>(GetGNamesAddress());
    return (!this->AreGObjectsValid() || !this->AreGNamesValid());
}

auto
EngineValidator::FindPattern(HMODULE module, const unsigned char* pattern, const char* mask) -> uintptr_t {
    MODULEINFO info = {};
    GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(MODULEINFO));

    uintptr_t start = reinterpret_cast<uintptr_t>(module);
    size_t length = info.SizeOfImage;

    size_t pos = 0;
    size_t maskLength = std::strlen(mask) - 1;

    for (uintptr_t retAddress = start; retAddress < start + length; retAddress++) {
        if (*reinterpret_cast<unsigned char*>(retAddress) == pattern[pos] || mask[pos] == '?') {
            if (pos == maskLength) {
                return (retAddress - maskLength);
            }
            pos++;
        } else {
            retAddress -= pos;
            pos = 0;
        }
    }
    return NULL;
}

auto
EngineValidator::GetGNamesAddress() -> uintptr_t {
    unsigned char GNamesPattern[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x35\x25\x02\x00";
    char GNamesMask[] = "??????xx??xxxxxx";

    uintptr_t GNamesAddress = FindPattern(GetModuleHandleW(L"RocketLeague.exe"), GNamesPattern, GNamesMask);

    return GNamesAddress;
}

auto
EngineValidator::GetGObjectsAddress() -> uintptr_t {
    return GetGNamesAddress() + 0x48;
}

auto
EngineValidator::AreGObjectsValid() -> bool {
    if (GObjects && !GObjects->empty() && GObjects->capacity() > GObjects->size()) {
        if (GObjects->at(0)->GetFullName() == "Class Core.Config_ORS") {
            return true;
        }
    }
    return false;
}

auto
EngineValidator::AreGNamesValid() -> bool {
    if (GNames && !GNames->empty() && GNames->capacity() > GNames->size()) {
        if (FName(0).ToString() == "None") {
            return true;
        }
    }
    return false;
}
