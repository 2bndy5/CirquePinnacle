#ifndef ARDUINO
    #include "gpio.h"
    #include <stdlib.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/types.h>
    #include <sys/stat.h>

namespace cirque_pinnacle_arduino_wrappers {

std::map<int, GPIOfdCache_t> GPIOClass::cache;

GPIOClass::GPIOClass()
{
}

GPIOClass::~GPIOClass()
{
}

void GPIOClass::open(pinnacle_gpio_t port, bool direction)
{
    FILE* f;
    f = fopen("/sys/class/gpio/export", "w");
    if (f == NULL) {
        throw GPIOException("Can't export GPIO pin. Check access rights.");
    }
    fprintf(f, "%d\n", port);
    fclose(f);

    int counter = 0;
    char file[128];
    sprintf(file, "/sys/class/gpio/gpio%d/direction", port);

    while ((f = fopen(file, "w")) == NULL) {
        //Wait 10 seconds for the file to be accessible if not open on first attempt
        sleep(1);
        counter++;
        if (counter > 10) {
            throw GPIOException("Can't access GPIO pin direction. Check access rights.");
        }
    }
    int l = direction ? fprintf(f, "in\n") : fprintf(f, "out\n");
    if (!(l == 3 || l == 4)) {
        fclose(f);
        throw GPIOException("Can't set direction of GPIO pin. Check access rights.");
    }
    fclose(f);

    // Caches the GPIO descriptor;
    sprintf(file, "/sys/class/gpio/gpio%d/value", port);
    int flags = direction ? O_RDONLY : O_WRONLY;
    int fd = ::open(file, flags);
    if (fd < 0) {
        throw GPIOException("Can't initialize GPIO pin. Check access rights.");
    }
    else {
        cache[port] = fd; // cache the fd;
        lseek(fd, SEEK_SET, 0);
    }
}

void GPIOClass::close(pinnacle_gpio_t port)
{
    std::map<int, GPIOfdCache_t>::iterator i;
    i = cache.find(port);
    if (i != cache.end()) {
        close(i->second); // close the cached fd
        cache.erase(i);   // Delete cache entry
    }
    // Do unexport
    FILE* f;
    f = fopen("/sys/class/gpio/unexport", "w");
    if (f != NULL) {
        fprintf(f, "%d\n", port);
        fclose(f);
    }
}

bool GPIOClass::read(pinnacle_gpio_t port)
{
    int fd;
    std::map<int, GPIOfdCache_t>::iterator i = cache.find(port);
    if (i == cache.end()) {
        throw GPIOException("GPIO pin not initialized.");
    }
    else {
        fd = i->second;
    }

    char c;
    if (lseek(fd, 0, SEEK_SET) == 0 && ::read(fd, &c, 1) == 1) {
        return (c == '0') ? 0 : 1;
    }
    else {
        throw GPIOException("Can't access GPIO pin");
    }
}

void GPIOClass::write(pinnacle_gpio_t port, const char* value)
{
    GPIOfdCache_t fd;
    std::map<int, GPIOfdCache_t>::iterator i = cache.find(port);
    if (i == cache.end()) {
        throw GPIOException("GPIO pin not initialized.");
    }
    else {
        fd = i->second;
    }

    if (lseek(fd, 0, SEEK_SET) != 0) {
        throw GPIOException("Can't access GPIO pin");
    }
    int l = ::write(fd, value, 2);
    if (l != 2) {
        throw GPIOException("Can't access GPIO pin");
    }
}

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
