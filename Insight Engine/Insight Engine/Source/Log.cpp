#include "Pch.h"
#include "Log.h"

namespace IS {

    std::shared_ptr<Logger> Log::core_logger;
    std::shared_ptr<Logger> Log::client_logger;

    void Log::init(bool enable_output) {
        core_logger = std::make_shared<Logger>("Insight");
        core_logger->setLogLevel(aLogLevel::Trace);
        client_logger = std::make_shared<Logger>("Fragments");
        client_logger->setLogLevel(aLogLevel::Trace);
        if (enable_output) {
            core_logger->enableFileOutput();
            client_logger->enableFileOutput();
        }
    }

} // end namespace IS
