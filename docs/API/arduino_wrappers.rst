Arduino-like Wrappers
=====================

This library ships with wrapper classes for non-Arduino platforms as a utility for portability.
All Arduino-like API exists in a library-specific namespace, ``cirque_pinnacle_arduino_wrappers``.

.. cpp:namespace:: cirque_pinnacle_arduino_wrappers

.. code-block:: cpp

    // an alias for the lib-specific namespace
    namespace arduino = cirque_pinnacle_arduino_wrappers;

    // the default I2C bus
    arduino::Wire;

    // the default SPI bus
    arduino::SPI;

.. info:: Generic API documented

    The following API is documented from the template used to extend portability to non-Arduino
    platforms (like Linux and the Pico SDK). Therefor, the following API is not entirely representative
    of the actual API implemented for any specific platform. Although, it is very similar to the
    implementation exposed for the Linux platform using the ``linux_kernel`` driver.

Arduino TwoWire
---------------

This is library-specific API wrapping the Arduino I2C interface.

.. cpp-apigen-group:: arduino-i2c

Arduino SPIClass
----------------

This is library-specific API wrapping the Arduino SPI interface.

.. cpp-apigen-group:: arduino-spi

Arduino GPIOClass
------------------

This is library-specific API wrapping the Arduino GPIO interface.

.. cpp-apigen-group:: arduino-gpio
