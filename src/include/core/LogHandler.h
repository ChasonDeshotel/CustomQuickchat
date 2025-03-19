// Static member initialization
std::ofstream Logger::logFile;
LogLevel Logger::minLevel = LogLevel::INFO;
bool Logger::categoryEnabled[static_cast<int>(LogCategory::PERFORMANCE) + 1] = { true };
bool Logger::consoleOutput = true;
std::string Logger::logFilePath = "mod_log.txt";
bool Logger::initialized = false;

// Example usage in hooks:
#ifdef DEBUG
#    define LOG_DEBUG(category, message) Logger::LogDebug(category, message)
#    define LOG_PERF_BEGIN(name) auto start_##name = std::chrono::high_resolution_clock::now()
#    define LOG_PERF_END(name)                                                                                            \
        {                                                                                                                 \
            auto end_##name = std::chrono::high_resolution_clock::now();                                                  \
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_##name - start_##name).count();     \
            Logger::LogDebug(LogCategory::PERFORMANCE, std::string(#name) + " took " + std::to_string(duration) + " μs"); \
        }
#else
#    define LOG_DEBUG(category, message)
#    define LOG_PERF_BEGIN(name)
#    define LOG_PERF_END(name)
#endif

#define LOG_INFO(category, message) Logger::LogInfo(category, message)
#define LOG_WARNING(category, message) Logger::LogWarning(category, message)
#define LOG_ERROR(category, message) Logger::LogError(category, message)
#define LOG_FATAL(category, message) Logger::LogFatal(category, message)