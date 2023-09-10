#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_PROFILING_TIMER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_PROFILING_TIMER_H

#include <chrono>

namespace IS {

    class Timer {
    public:
        Timer(const char* name);
        ~Timer();

        float GetDeltaTime() const;
        void Stop();

    private:
        struct ProfileResult {
            const char* name;
            std::chrono::duration<float> time;
        };

        ProfileResult result;
        std::chrono::time_point<std::chrono::steady_clock> start_timepoint;
        bool stopped;
    };

} // end namespace IS

// For tracking time taken to run in a scope
#ifdef IS_ENABLE_PROFILE
    #define IS_PROFILE_SCOPE(name) IS::Timer timer##__LINE__(name)
    #define IS_PROFILE_FUNCTION() IS_PROFILE_SCOPE(__FUNCSIG__)
#else
    #define IS_PROFILE_SCOPE(name) ((void)0)
    #define IS_PROFILE_FUNCTION() ((void)0)
#endif // IS_ENABLE_PROFILE

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_PROFILING_TIMER_H
