/*!
 * \file Log.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 *      This source file defines the implementation for class Log which encapsulates
 *      the functionalities of an Engine-side log and a Client-side log.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Log.h"

namespace IS {

    std::shared_ptr<Logger> Log::mCoreLogger;
    std::shared_ptr<Logger> Log::mClientLogger;

    void Log::Init(bool enable_output) {
        mCoreLogger = std::make_shared<Logger>("Insight");
        mCoreLogger->SetLogLevel(aLogLevel::Trace);
        mClientLogger = std::make_shared<Logger>("Fragments");
        mClientLogger->SetLogLevel(aLogLevel::Trace);
        if (enable_output) {
            mCoreLogger->EnableFileOutput();
            mClientLogger->EnableFileOutput();
        }
    }

} // end namespace IS
