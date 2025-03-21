#pragma once
#include "LogHandler.h"
#include "LogTypes.h"
#include <memory>

// NOLINTBEGIN
extern std::shared_ptr<LogHandler> initializeLogger();
extern std::shared_ptr<LogHandler> logger;
// NOLINTEND
