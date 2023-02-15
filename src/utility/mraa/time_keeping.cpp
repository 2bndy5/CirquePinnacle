#ifndef ARDUINO
    #include <time.h>
    #include <chrono>
    #include <sys/time.h>
    #include "time_keeping.h"

namespace cirque_pinnacle_arduino_wrappers {

void __msleep(int milisec)
{
    struct timespec req = {0};
    req.tv_sec = 0;
    req.tv_nsec = milisec * 1000000L;
    nanosleep(&req, (struct timespec*)NULL);
    //usleep(milisec*1000);
}

void __usleep(int milisec)
{
    struct timespec req = {0};
    req.tv_sec = 0;
    req.tv_nsec = milisec * 1000L;
    nanosleep(&req, (struct timespec*)NULL);
    //usleep(milisec);
}

auto start = std::chrono::steady_clock::now();

uint32_t __millis()
{
    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
