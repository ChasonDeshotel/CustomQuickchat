#pragma once

class EngineValidator {
  public:
    void init();
    static uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask);
    static uintptr_t GetGNamesAddress();
    static uintptr_t GetGObjectsAddress();
    static void InitGlobals();
    static bool AreGObjectsValid();
    static bool AreGNamesValid();
    static bool CheckGlobals();
};