#include "Pch.h"
#include "Log.h"

namespace IS {

    std::shared_ptr<Logger> Log::core_logger;
    std::shared_ptr<Logger> Log::client_logger;

    void Log::init() {
        core_logger = std::make_shared<Logger>("INSIGHT");
        core_logger->setLogLevel(aLogLevel::Trace);
        client_logger = std::make_shared<Logger>("GAME");
        core_logger->setLogLevel(aLogLevel::Trace);
    }

    void Log::enableFileOutput() {
        core_logger->enableFileOutput();
        client_logger->enableFileOutput();
    }

} // end namespace IS
