#include <memory>

#include "DependencyContainer.h"
#include "LogGlobal.h"
#include "LogHandler.h"
#include "sink/Console.h"
#include "sink/File.h"

// Define BUILDING_DLL when compiling this file
#define BUILDING_DLL
#include "LogGlobal.h"

// NOLINTBEGIN
// Define the global logger variable
std::shared_ptr<LogHandler> logger = nullptr;
// NOLINTEND

// Implementation of the initialization function
std::shared_ptr<LogHandler>
initializeLogger() {
    if (!logger) {
        auto& container = DependencyContainer::getInstance();
        container.registerType<LogHandler, LogHandler>(DependencyContainer::Lifetime::Singleton);
        logger = container.resolve<LogHandler>();
        logger->setLogLevel(LogLevel::LOG_DEBUG);

        logger->addSink(std::make_shared<ConsoleLogSink>());

        // fixme
        auto fileSink = std::make_shared<FileLogSink>("C:\\log-move-me.log");
        if (fileSink->isAvailable()) {
            logger->addSink(fileSink);
        }

        logger->info("Logger initialized", LogCategory::CORE);
    }
    return logger;
}
