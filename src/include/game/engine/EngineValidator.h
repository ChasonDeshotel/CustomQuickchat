#pragma once
#include "GameDefines.hpp"
#include "SdkHeaders.hpp"

class EngineValidator {
  public:
    EngineValidator() = default;
    ~EngineValidator() = default;

    EngineValidator(const EngineValidator&) = delete;
    auto operator=(const EngineValidator&) -> EngineValidator& = delete;
    EngineValidator(EngineValidator&&) = delete;
    auto operator=(EngineValidator&&) -> EngineValidator& = delete;

    bool init();

    static TArray<UObject*>* GetObjects() { return GObjects; }
    static TArray<FNameEntry*>* GetNames() { return GNames; }

    auto AreGObjectsValid() -> bool;
    auto AreGNamesValid() -> bool;

    auto FindPattern(HMODULE module, const unsigned char* pattern, const char* mask) -> uintptr_t;
    auto GetGNamesAddress() -> uintptr_t;
    auto GetGObjectsAddress() -> uintptr_t;
};