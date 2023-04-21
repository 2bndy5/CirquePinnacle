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
    #include <stdlib.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include "gpio.h"

namespace cirque_pinnacle_arduino_wrappers {

std::map<pinnacle_gpio_t, gpio_cache_fd_t> GPIOClass::cache;

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
    int l = direction ? fprintf(f, "out\n") : fprintf(f, "in\n");
    if (!(l == 3 || l == 4)) {
        fclose(f);
        throw GPIOException("Can't set direction of GPIO pin. Check access rights.");
    }
    fclose(f);

    // Caches the GPIO descriptor
    sprintf(file, "/sys/class/gpio/gpio%d/value", port);
    int flags = direction ? O_WRONLY : O_RDONLY;
    int fd = ::open(file, flags);
    if (fd < 0) {
        throw GPIOException("Can't initialize GPIO pin. Check access rights.");
    }
    else {
        cache[port] = fd; // cache the fd;
        lseek(fd, 0, SEEK_SET);
    }
}

void GPIOClass::close(pinnacle_gpio_t port)
{
    std::map<pinnacle_gpio_t, gpio_cache_fd_t>::iterator i;
    i = cache.find(port);
    if (i != cache.end()) {
        ::close(i->second); // close the cached fd
        cache.erase(i);     // Delete cache entry
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
    std::map<pinnacle_gpio_t, gpio_cache_fd_t>::iterator i = cache.find(port);
    if (i == cache.end()) {
        throw GPIOException("GPIO pin not initialized.");
    }
    else {
        fd = i->second;
    }

    if (lseek(fd, 0, SEEK_SET) != 0) {
        if (errno == EBADF)
            throw GPIOException("GPIO::read lseek() with an invalid file descriptor.");
        else if (errno == EINVAL)
            throw GPIOException("GPIO::read using invalid lseek(..., whence) value.");
        else if (errno == ENXIO)
            throw GPIOException("GPIO::read lseek(..., offset, whence) specifies position beyond end-of-file.");
        else if (errno == EOVERFLOW)
            throw GPIOException("GPIO::read lseek() resulting offset is out-of-bounds for a signed integer.");
        else if (errno == ESPIPE)
            throw GPIOException("GPIO::read lseek() file descriptor is associated with a pipe, socket, or FIFO.");
    }
    char c;
    if (::read(fd, &c, 1) == 1) {
        return (c == '0') ? 0 : 1;
    }
    else {
        throw GPIOException("Can't read GPIO pin");
    }
}

void GPIOClass::write(pinnacle_gpio_t port, const char* value)
{
    gpio_cache_fd_t fd;
    std::map<pinnacle_gpio_t, gpio_cache_fd_t>::iterator i = cache.find(port);
    if (i == cache.end()) {
        throw GPIOException("GPIO pin not initialized.");
    }
    else {
        fd = i->second;
    }

    if (lseek(fd, 0, SEEK_SET) != 0) {
        if (errno == EBADF)
            throw GPIOException("GPIO::write lseek() with an invalid file descriptor.");
        else if (errno == EINVAL)
            throw GPIOException("GPIO::write using invalid lseek(..., whence) value.");
        else if (errno == ENXIO)
            throw GPIOException("GPIO::write lseek(..., offset, whence) specifies position beyond end-of-file.");
        else if (errno == EOVERFLOW)
            throw GPIOException("GPIO::write lseek() resulting offset is out-of-bounds for a signed integer.");
        else if (errno == ESPIPE)
            throw GPIOException("GPIO::write lseek() file descriptor is associated with a pipe, socket, or FIFO.");
    }
    int l = ::write(fd, value, 2);
    if (l != 2) {
        throw GPIOException("Can't write to GPIO pin");
    }
}

} // namespace cirque_pinnacle_arduino_wrappers

#endif // !defined(ARDUINO)
