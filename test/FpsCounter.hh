#pragma once

#include <chrono>
#include <cstdint>

class FpsCounter {
public:
    using clock = std::chrono::steady_clock;

    void frame() {
        using namespace std::chrono;

        const auto now = clock::now();
        const auto dt = duration<float>(now - last_).count();
        last_ = now;

        if (dt > 0.0f) {
            fps_ = 1.0f / dt;
        }
    }

    float fps() const noexcept { return fps_; }

private:
    clock::time_point last_ = clock::now();
    float fps_ = 0.0f;
};
