/*!
 * \file Timer.cpp
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This source file defines the implementation for class EditorLayer which
 * encapsulates the functionalities of a level editor layer.
 * 
 * \copyright
 * All content (C) 2024 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "Pch.h"
#include "Timer.h"

namespace IS {

    Timer::Timer(std::string const& name, bool console_print) : mStopped(false), mToPrint(console_print)
    {
        mResult.mName = name;
        mStartTime = std::chrono::high_resolution_clock::now();
    }

    Timer::~Timer()
    {
        if (!mStopped)
            Stop();
    }

    float Timer::GetDeltaTime() const { return mResult.mTime.count(); }

    void Timer::Stop()
    {
        auto end_timepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(mStartTime).time_since_epoch();
        auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(end_timepoint).time_since_epoch();

        mResult.mTime = std::chrono::duration<float>(end - start);
        mStopped = true;

        if (mToPrint)
            IS_CORE_TRACE("{}: {:.3f} ms", mResult.mName, GetDeltaTime() * 1000);
    }

} // end namespace IS
