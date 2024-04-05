#pragma once

#include <chrono>

class Clock {
private:
    using clock = std::chrono::steady_clock;
    using time_point = std::chrono::time_point<clock>;

    time_point start_time;

public:
    Clock() : start_time(clock::now()) {}

    void start() {
        start_time = clock::now();
    }

    double getElapsedTime() const {
        auto end_time = clock::now();
        auto duration = end_time - start_time;
        return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    }

    double restart() {
        auto end_time = clock::now();
        auto duration = end_time - start_time;
        start_time = end_time;
        return std::chrono::duration_cast<std::chrono::duration<double>>(duration).count();
    }
};
