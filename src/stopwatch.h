#pragma once

#include <cstdint>
#include <chrono>

template<typename Precision>
class Stopwatch {
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point pauseTime;

    bool paused;

    uint64_t pausedTime;

   public:
    Stopwatch() {
        start();
    }

    void start() {
        this->startTime  = std::chrono::high_resolution_clock::now();
        this->pausedTime = 0;
        this->paused     = false;
    }

    void reset() {
        start();
    }

    uint64_t elapsed() const {
        uint64_t pausedTime = this->pausedTime;
        if (this->paused)
            pausedTime += std::chrono::duration_cast<Precision>(std::chrono::high_resolution_clock::now() - pauseTime).count();
        return std::chrono::duration_cast<Precision>(std::chrono::high_resolution_clock::now() - startTime).count() - pausedTime;
    }

    void pause() {
        this->paused    = true;
        this->pauseTime = std::chrono::high_resolution_clock::now();
    }
    void resume() {
        this->paused = false;
        this->pausedTime += std::chrono::duration_cast<Precision>(std::chrono::high_resolution_clock::now() - pauseTime).count();
    }
};