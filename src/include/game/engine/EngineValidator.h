#pragma once

class EngineValidator {
  public:
    EngineValidator();
    ~EngineValidator() = default;

    EngineValidator(const EngineValidator&) = delete;
    auto operator=(const EngineValidator&) -> EngineValidator& = delete;
    EngineValidator(EngineValidator&&) = delete;
    auto operator=(EngineValidator&&) -> EngineValidator& = delete;

    void init();

    static void InitGlobals();
    static bool AreGObjectsValid();
    static bool AreGNamesValid();
    static bool CheckGlobals();

    static uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask);
    static uintptr_t GetGNamesAddress();
    static uintptr_t GetGObjectsAddress();
};