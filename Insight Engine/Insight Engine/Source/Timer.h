/*!
 * \file Timer.h
 * \author Guo Yiming, yiming.guo@digipen.edu
 * \par Course: CSD2401
 * \date 23-09-2023
 * \brief
 * This header file declares the interface for class Timer, which encapsulates
 * the functionalities of a stopwatch.
 * 
 * \copyright
 * All content (C) 2023 DigiPen Institute of Technology Singapore.
 * All rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior written
 * consent of DigiPen Institute of Technology is prohibited.
 *____________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GAM200_INSIGHT_ENGINE_DEBUG_PROFILING_TIMER_H
#define GAM200_INSIGHT_ENGINE_DEBUG_PROFILING_TIMER_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <chrono>

namespace IS {

    /*!
     * \brief The Timer class measures time intervals and provides profiling capabilities.
     *
     * The Timer class allows for measuring time intervals and provides options for console printing and profiling.
     */
    class Timer {
    public:
        /*!
         * \brief Constructs a Timer object.
         * \param name The name of the timer.
         * \param console_print Flag indicating whether to print timer information to the console (default is true).
         */
        Timer(std::string const& name, bool console_print = true);

        /*!
         * \brief Destroys the Timer object.
         */
        ~Timer();

        /*!
         * \brief Gets the time elapsed since the timer was started.
         * \return The time elapsed in seconds since the timer was started.
         */
        float GetDeltaTime() const;

        /*!
         * \brief Stops the timer.
         *
         * This function stops the timer, preventing further time accumulation.
         */
        void Stop();

    private:
        /*!
         * \brief Structure to store profile results.
         */
        struct ProfileResult {
            std::string mName; ///< The name associated with the profile result.
            std::chrono::duration<float> mTime; ///< The time duration for profiling.
        };

        ProfileResult mResult; ///< The profile result.
        std::chrono::time_point<std::chrono::steady_clock> mStartTime; ///< The start time point.
        bool mStopped; ///< Flag indicating whether the timer is stopped.
        bool mToPrint; ///< Flag indicating whether to print timer information to the console.
    };

} // end namespace IS

/*                                                                     macros
----------------------------------------------------------------------------- */
#define IS_PROFILE_SCOPE(name) IS::Timer timer##__LINE__(name) // time within a scope
#define IS_PROFILE_FUNCTION() IS_PROFILE_SCOPE(__FUNCTION__)   // time within a function

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_PROFILING_TIMER_H
