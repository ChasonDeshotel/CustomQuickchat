#pragma once
#include <memory>
#include <string>

class LogSink {
  public:
    virtual ~LogSink() = default;

    virtual void write(const std::string& formattedMessage) = 0;
    virtual bool isAvailable() const { return true; }
    virtual void flush() {}
};