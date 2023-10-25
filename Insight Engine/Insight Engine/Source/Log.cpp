/*!
 * \file Log.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for class Log which encapsulates
 * the functionalities of an Engine-side log and a Client-side log.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Log.h"

namespace IS {

    // Static instances
    std::shared_ptr<Logger> Log::mCoreLogger;
    std::shared_ptr<Logger> Log::mClientLogger;

    void Log::Init(bool enable_output)
    {
        mCoreLogger = std::make_shared<Logger>("Insight");
        mClientLogger = std::make_shared<Logger>("Fragments");

    #if defined(_DEBUG)
        mCoreLogger->SetLogLevel(aLogLevel::LOGLEVEL_TRACE);
        mClientLogger->SetLogLevel(aLogLevel::LOGLEVEL_TRACE);
    #else
        mCoreLogger->SetLogLevel(aLogLevel::LOGLEVEL_INFO);
        mClientLogger->SetLogLevel(aLogLevel::LOGLEVEL_INFO);
    #endif

        if (enable_output)
        {
            mCoreLogger->EnableFileOutput();
            mClientLogger->EnableFileOutput();
        }
    }

} // end namespace IS
