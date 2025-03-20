#pragma once
#include "LogSink.h"
#include <filesystem>
#include <fstream>
#include <mutex>

class FileLogSink : public LogSink {
  public:
    FileLogSink(const std::string& filePath, bool append = true)
      : logFilePath(filePath) {

        std::filesystem::path p(filePath);
        if (p.has_parent_path()) {
            std::filesystem::create_directories(p.parent_path());
        }

        logFile.open(filePath, append ? std::ios::app : std::ios::trunc);
    }

    ~FileLogSink() override {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void write(const std::string& formattedMessage) override {
        std::lock_guard<std::mutex> lock(fileMutex);
        if (logFile.is_open()) {
            logFile << formattedMessage << std::endl;
        }
    }

    bool isAvailable() const override { return logFile.is_open() && logFile.good(); }

    void flush() override {
        std::lock_guard<std::mutex> lock(fileMutex);
        if (logFile.is_open()) {
            logFile.flush();
        }
    }

  private:
    std::string logFilePath;
    std::ofstream logFile;
    std::mutex fileMutex;
};