#pragma once
#include <filesystem>
#include <fstream>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

#include "LogSink.h"
#include "LogTypes.h"

class LogHandler {
  public:
    LogHandler();
    LogHandler(const LogHandler&) = delete;
    LogHandler(LogHandler&&) = delete;
    auto operator=(const LogHandler&) -> LogHandler& = delete;
    auto operator=(LogHandler&&) -> LogHandler& = delete;
    ~LogHandler();
    static auto getInstance() -> LogHandler&;

    auto addSink(std::shared_ptr<LogSink> sink) -> void;

    auto log(const std::string& message, LogCategory category, LogLevel level = LogLevel::LOG_INFO) -> void;
    auto debug(const std::string& message, LogCategory category) -> void;
    auto info(const std::string& message, LogCategory category) -> void;
    auto warn(const std::string& message, LogCategory category) -> void;
    auto error(const std::string& message, LogCategory category) -> void;

    auto setLogPath(const std::string& path) -> void;
    auto setLogLevel(LogLevel level) -> void;
    auto setCategoryEnabled(LogCategory category, bool enabled) -> void;

  private:
    std::ofstream logfile;
    std::optional<std::filesystem::path> logPath;
    LogLevel currentLogLevel;
    std::unordered_map<LogCategory, bool> enabledCategories;
    std::mutex logMutex;
    auto logLevelToString(LogLevel level) -> std::string;

    std::vector<std::shared_ptr<LogSink>> sinks;
};