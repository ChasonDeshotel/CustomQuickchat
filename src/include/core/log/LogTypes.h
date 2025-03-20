#pragma once
#include <string>

enum class LogLevel { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

enum class LogCategory { CORE, UI, NETWORK, HOOKS, STT, CHAT, BAKKESMOD, MEMORY, PERFORMANCE };

inline std::string
logCategoryToString(LogCategory category) {
    switch (category) {
        case LogCategory::CORE: return "CORE";
        case LogCategory::UI: return "UI";
        case LogCategory::NETWORK: return "NETWORK";
        case LogCategory::HOOKS: return "HOOKS";
        case LogCategory::STT: return "STT";
        case LogCategory::CHAT: return "CHAT";
        case LogCategory::BAKKESMOD: return "BAKKESMOD";
        case LogCategory::MEMORY: return "MEMORY";
        case LogCategory::PERFORMANCE: return "PERFORMANCE";
        default: return "UNKNOWN";
    }
}