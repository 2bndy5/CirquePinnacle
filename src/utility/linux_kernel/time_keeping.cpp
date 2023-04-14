/*
 * Copyright (c) 2023 Brendan Doherty (2bndy5)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef ARDUINO
    #include <time.h>
    #include <chrono>
    #include <sys/time.h>
    #include "time_keeping.h"

    #ifdef __cplusplus
extern "C" {
    #endif

namespace cirque_pinnacle_arduino_wrappers {

    void __msleep(int milliseconds)
    {
        struct timespec req;
        req.tv_sec = (time_t)milliseconds / 1000;
        req.tv_nsec = (milliseconds % 1000) * 1000000L;
        //nanosleep(&req, (struct timespec *)NULL);
        clock_nanosleep(CLOCK_REALTIME, 0, &req, NULL);
    }

    void __usleep(int microseconds)
    {
        struct timespec req;
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

} // namespace cirque_pinnacle_arduino_wrappers

    #ifdef __cplusplus
}
    #endif

#endif // !defined(ARDUINO)
