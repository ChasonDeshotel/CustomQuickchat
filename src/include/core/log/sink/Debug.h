#ifdef _WIN32
#    pragma once
#    include "LogSink.h"
#    include <Windows.h>

// For Windows OutputDebugString
class DebugOutputSink : public LogSink {
  public:
    void write(const std::string& formattedMessage) override { OutputDebugStringA((formattedMessage + "\n").c_str()); }
};
#endif