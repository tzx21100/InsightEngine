#include "Pch.h"
#include "Timer.h"

namespace IS {

    Timer::Timer(const char* name) : stopped(false) {
        result = ProfileResult(name);
        start_timepoint = std::chrono::high_resolution_clock::now();
    }

    Timer::~Timer() {
        if (!stopped)
            Stop();
    }

    float Timer::GetDeltaTime() const {
        return result.time.count();
    }

    void Timer::Stop() {
        auto end_timepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::nanoseconds>(start_timepoint).time_since_epoch();
        auto end = std::chrono::time_point_cast<std::chrono::nanoseconds>(end_timepoint).time_since_epoch();

        result.time = std::chrono::duration<float>(end - start);
        stopped = true;

    #ifdef IS_ENABLE_PROFILE
        IS_CORE_DEBUG("{} : {:.3f} ms", result.name, GetDeltaTime() * 1000);
    #endif // IS_ENABLE_PROFILE
    }

} // end namespace IS
