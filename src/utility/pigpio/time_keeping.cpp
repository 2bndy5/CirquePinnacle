#ifndef ARDUINO
    #include <time.h>
    #include <chrono>
    #include "time_keeping.h"

namespace cirque_pinnacle_arduino_wrappers {

/**********************************************************************/
/**
 * This function is added in order to simulate arduino delay() function
 * @param milisec
 */
void __msleep(int milisec)
{
    struct timespec req; // = {0};
    req.tv_sec = (time_t)milisec / 1000;
    req.tv_nsec = (milisec % 1000) * 1000000L;
    //nanosleep(&req, (struct timespec *)NULL);
    clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL);
}

void __usleep(int microsec)
{
    struct timespec req; // = {0};
    req.tv_sec = (time_t)microsec / 1000000;
    req.tv_nsec = (microsec / 1000000) * 1000;
    //nanosleep(&req, (struct timespec *)NULL);
    clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL);
}

/**
 * This function is added in order to simulate arduino millis() function
 */

bool timerStarted = false;

void __start_timer()
{
}

auto start = std::chrono::steady_clock::now();

uint32_t __millis()
{

    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)