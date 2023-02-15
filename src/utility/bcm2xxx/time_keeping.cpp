#ifndef ARDUINO
    #include "time_keeping.h"
    #include <chrono>

namespace cirque_pinnacle_arduino_wrappers {

auto start = std::chrono::steady_clock::now();

uint32_t __millis()
{
    auto end = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
