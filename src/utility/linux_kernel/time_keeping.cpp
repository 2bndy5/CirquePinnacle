#ifndef ARDUINO
    #include <time.h>
    #include <chrono>
    #include <sys/time.h>
    #include "time_keeping.h"

namespace cirque_pinnacle_arduino_wrappers {

    #ifdef __cplusplus
extern "C" {
    #endif

void __msleep(int milliseconds)
{
    struct timespec req; // = {0};
    req.tv_sec = (time_t)milliseconds / 1000;
    req.tv_nsec = (milliseconds % 1000) * 1000000L;
    //nanosleep(&req, (struct timespec *)NULL);
    clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL);
}

void __usleep(int microseconds)
{
    struct timespec req; // = {0};
    req.tv_sec = (time_t)microseconds / 1000000;
    req.tv_nsec = (microseconds / 1000000) * 1000;
    //nanosleep(&req, (struct timespec *)NULL);
    clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL);
}

auto start = std::chrono::steady_clock::now();

uint32_t __millis()
{
    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

    #ifdef __cplusplus
}
    #endif

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
