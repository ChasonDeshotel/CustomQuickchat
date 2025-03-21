#pragma once
#include "LogHandler.h"
#include "LogTypes.h"
#include <memory>

// NOLINTBEGIN
// Make sure these are properly exported/imported
#ifdef _WIN32
  #ifdef BUILDING_DLL
    #define LOG_API __declspec(dllexport)
  #else
    #define LOG_API __declspec(dllimport)
  #endif
#else
  #define LOG_API
#endif

LOG_API std::shared_ptr<LogHandler> initializeLogger();
LOG_API extern std::shared_ptr<LogHandler> logger;
// NOLINTEND
