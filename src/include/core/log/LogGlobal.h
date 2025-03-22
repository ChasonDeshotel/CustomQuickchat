#pragma once
#include "LogHandler.h"
#include <memory>

// NOLINTBEGIN
// Make sure these are properly exported/imported
std::shared_ptr<LogHandler>
initializeLogger();
extern std::shared_ptr<LogHandler> logger;
// NOLINTEND
