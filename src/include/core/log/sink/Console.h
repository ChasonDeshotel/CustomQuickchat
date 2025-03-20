#pragma once
#include "LogSink.h"
#include <iostream>

class ConsoleLogSink : public LogSink {
  public:
    void write(const std::string& formattedMessage) override { std::cout << formattedMessage << std::endl; }

    void flush() override { std::cout.flush(); }
};