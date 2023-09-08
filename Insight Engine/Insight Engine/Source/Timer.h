#ifndef GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_TIMER_H
#define GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_TIMER_H

#include <chrono>

namespace IS {

    struct ProfileResult {
        const char* name;
        float time;
    };

    class Timer {
    public:
        Timer(const char* new_name);
        ~Timer();

        void Stop();

    private:
        const char* name;
        std::chrono::time_point<std::chrono::steady_clock> start_timepoint;
        bool stopped;
    };

}

#endif // !GAM200_INSIGHT_ENGINE_SOURCE_DEBUG_TIMER_H
