#include <xTimer.h>
#include <chrono>

unsigned long long xTimer::getNativeTime() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}
float xTimer::getPassedTimeMs(unsigned long long prev, unsigned long long now) {
    return now < prev ? 0.0f : (now - prev) * 0.000001f;
}