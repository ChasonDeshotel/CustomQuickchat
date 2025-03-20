#include "EngineValidator.h"

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

void
EngineValidator::InitGlobals() {
    GObjects = reinterpret_cast<TArray<UObject*>*>(GetGObjectsAddress());
    GNames = reinterpret_cast<TArray<FNameEntry*>*>(GetGNamesAddress());
}

auto
EngineValidator::AreGObjectsValid() -> bool {
    if (UObject::GObjObjects()->size() > 0 && UObject::GObjObjects()->capacity() > UObject::GObjObjects()->size()) {
        if (UObject::GObjObjects()->at(0)->GetFullName() == "Class Core.Config_ORS") {
            return true;
        }
    }
    return false;
}

auto
EngineValidator::AreGNamesValid() -> bool {
    if (FName::Names()->size() > 0 && FName::Names()->capacity() > FName::Names()->size()) {
        if (FName(0).ToString() == "None") {
            return true;
        }
    }
    return false;
}

auto
EngineValidator::CheckGlobals() -> bool {
    if (!GObjects || !GNames || !AreGObjectsValid() || !AreGNamesValid()) {
        LOG("(onLoad) Error: RLSDK classes are wrong... plugin needs an update :(");
        LOG(std::to_string(!GObjects) + ", " + std::to_string(!GNames));
        return false;
    }

    LOG("Globals Initialized :)");
    return true;
}
