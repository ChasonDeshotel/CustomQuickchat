#include "LogHandler.h"
#include <chrono>
#include <iomanip>
#include <sstream>

LogHandler::LogHandler()
  : currentLogLevel(LogLevel::LOG_INFO) {
    // Initialize all categories as enabled by default
    for (int i = 0; i <= static_cast<int>(LogCategory::PERFORMANCE); i++) {
        enabledCategories[static_cast<LogCategory>(i)] = true;
    }
}

auto
LogHandler::log(const std::string& message, LogCategory category, LogLevel level) -> void {
    if (level < currentLogLevel || !enabledCategories[category]) {
        return;
    }

    std::lock_guard<std::mutex> lock(logMutex);

    if (!logfile.is_open() && logPath.has_value()) {
        logfile.open(logPath.value(), std::ios::app);
    }

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    // fixme
    std::tm tm = *std::localtime(&time);

    std::stringstream logEntry;
    logEntry << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] "
             << "[" << logLevelToString(level) << "][" << logCategoryToString(category) << "] " << message;

    if (logfile.is_open()) {
        logfile << logEntry.str() << std::endl;
        logfile.flush();
    }

    // Optional console output
    std::cout << logEntry.str() << std::endl;
}

auto
LogHandler::debug(const std::string& message, LogCategory category) -> void {
    log(message, category, LogLevel::LOG_DEBUG);
}

auto
LogHandler::info(const std::string& message, LogCategory category) -> void {
    log(message, category, LogLevel::LOG_INFO);
}

auto
LogHandler::warn(const std::string& message, LogCategory category) -> void {
    log(message, category, LogLevel::LOG_WARN);
}

auto
LogHandler::error(const std::string& message, LogCategory category) -> void {
    log(message, category, LogLevel::LOG_ERROR);
}

auto
LogHandler::setCategoryEnabled(LogCategory category, bool enabled) -> void {
    std::lock_guard<std::mutex> lock(logMutex);
    enabledCategories[category] = enabled;

    // Log this change if possible
    if (enabled) {
        log("Enabled logging for category: " + logCategoryToString(category), LogCategory::CORE, LogLevel::LOG_INFO);
    } else {
        log("Disabled logging for category: " + logCategoryToString(category), LogCategory::CORE, LogLevel::LOG_INFO);
    }
}

// Convenience macros
#ifdef DEBUG
#    define LOG_DEBUG(category, message) LogHandler::getInstance().debug(message, category)
#    define LOG_PERF_BEGIN(name) auto start_##name = std::chrono::high_resolution_clock::now()
#    define LOG_PERF_END(name)                                                                                        \
        {                                                                                                             \
            auto end_##name = std::chrono::high_resolution_clock::now();                                              \
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_##name - start_##name).count(); \
            LogHandler::getInstance().debug(                                                                          \
                std::string(#name) + " took " + std::to_string(duration) + " μs", LogCategory::PERFORMANCE);          \
        }
#else
#    define LOG_DEBUG(category, message)
#    define LOG_PERF_BEGIN(name)
#    define LOG_PERF_END(name)
#endif

#define LOG_INFO(category, message) LogHandler::getInstance().info(message, category)
#define LOG_WARN(category, message) LogHandler::getInstance().warn(message, category)
#define LOG_ERROR(category, message) LogHandler::getInstance().error(message, category)
