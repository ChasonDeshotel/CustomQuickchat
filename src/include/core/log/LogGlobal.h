#pragma once
#include "LogHandler.h"
#include <memory>

// NOLINTBEGIN
extern void
initializeLogger();
extern std::shared_ptr<LogHandler> logger;
// NOLINTEND