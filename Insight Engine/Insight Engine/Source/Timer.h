#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_PROFILING_TIMER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_PROFILING_TIMER_H

#include <chrono>

namespace IS {

    class Timer {
    public:
        Timer(std::string const& name, bool console_print = true);
        ~Timer();

        float GetDeltaTime() const;
        void Stop();

    private:
        struct ProfileResult {
            std::string name;
            std::chrono::duration<float> time;
        };

        ProfileResult result;
        std::chrono::time_point<std::chrono::steady_clock> start_timepoint;
        bool stopped;
        bool print;
    };

} // end namespace IS

// For tracking time taken to run in a scope
#define IS_PROFILE_SCOPE(name) IS::Timer timer##__LINE__(name)
#define IS_PROFILE_FUNCTION() IS_PROFILE_SCOPE(__FUNCTION__)

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_PROFILING_TIMER_H
