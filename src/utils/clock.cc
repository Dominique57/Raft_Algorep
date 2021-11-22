#include "utils/clock.hh"

#include <unistd.h>

#include "wrappers/debug/print_log.hh"

namespace Clock {
    Clock::Clock(const SPEED_TYPE& speed_)
        : speed(speed_)
    {}

    void Clock::wait() const {
        if (speed == HIGH)
            return;

        Log::log("Going to sleep !");
        usleep(1000 * 250 * speed);
        Log::log("Waking up !");
    }
}
