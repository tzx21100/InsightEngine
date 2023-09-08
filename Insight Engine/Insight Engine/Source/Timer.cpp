#include "Pch.h"
#include "Timer.h"

namespace IS {

    Timer::Timer(const char* new_name) : name(new_name), stopped(false) {}

    Timer::~Timer() {
        if (!stopped)
            Stop();
    }

    void Timer::Stop() {
        auto end_timepoint = std::chrono::steady_clock::now();
        long long start = std::chrono::time_point_cast<std::chrono::milliseconds>(start_timepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::milliseconds>(end_timepoint).time_since_epoch().count();

        long long elapsed_time = start - end;

        stopped = true;
    }
}