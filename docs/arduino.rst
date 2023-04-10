Arduino Support
===============

This library was primarily written for the Arduino IDE. Therefore, it uses the SPI and I2C bus
implementations from the Arduino framework (as provided by various board support -- AKA "Arduino
Cores").

Using a non-default SPI or I2C bus
----------------------------------

.. md-tab-set::

    .. md-tab-item:: SPI

        It is possible to use a specific SPI bus on boards that expose more than 1 SPI bus. As noted above,
        the ``SPIClass`` interface may vary depending on the targeted board.

        The following code snippet exemplifies the necessary steps:

        .. code-block:: cpp
            :class: annotated-with-numbers
            :emphasize-lines: 7,18,20

            #include <CirquePinnacle.h>
            #include <SPI.h>

            #define SS_PIN 2
            #define DR_PIN 7

            SPIClass my_spi_obj; // (1)!

            PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);

            void begin()
            {
              Serial.begin(115200);
              while (!Serial) {
                // some boards need this for the prompts to show immediately after boot
              }

              my_spi_obj.begin(); // (2)!

              if (!trackpad.begin(&my_spi_obj)) { // (3)!
                Serial.println(F("Cirque Pinnacle not responding!"));
                while (true) {} // hold program in infinite loop
              }

              // continue program as usual...
            }

        .. code-annotations::
            1. Instantiate a ``SPIClass`` object for use with a specific SPI bus.
            2. Initialize the SPI bus using ``SPIClass::begin()`` method. The arguments passed to this function
               may differ depending on the Arduino core used (for a certain family of boards).

               Developers are encouraged to identify the use of ``SPIClass::begin()`` (or any
               ``SPIClass::set<pin>()``) as instructed by the utilized Arduino core's SPI library.
            3. Pass a reference to the instantiated ``SPIClass`` object to `PinnacleTouchSPI::begin()`.


    .. md-tab-item:: I2C

        It is possible to use a specific I2C bus on boards that expose more than 1 I2C bus. As noted above,
        the ``TwoWire`` interface may vary depending on the targeted board.

        The following code snippet exemplifies the necessary steps:

        .. code-block:: cpp
            :class: annotated-with-numbers
            :emphasize-lines: 6,17,19

            #include <CirquePinnacle.h>
            #include <Wire.h>

            #define DR_PIN 7

            TwoWire my_i2c_obj; // (1)!

            PinnacleTouchI2C trackpad(DR_PIN);

            void begin()
            {
              Serial.begin(115200);
              while (!Serial) {
                // some boards need this for the prompts to show immediately after boot
              }

              my_i2c_obj.begin(); // (2)!

              if (!trackpad.begin(&my_i2c_obj)) { // (3)!
                Serial.println(F("Cirque Pinnacle not responding!"));
                while (true) {} // hold program in infinite loop
              }

              // continue program as usual...
            }

        .. code-annotations::
            1. Instantiate a ``TwoWire`` object for use with a specific I2C bus.
            2. Initialize the I2C bus using ``TwoWire::begin()`` method. The arguments passed to this function
               may differ depending on the Arduino core used (for a certain family of boards).

               Developers are encouraged to identify the use of ``TwoWire::begin()`` (or any
               ``TwoWire::set<pin>()``) as instructed by the utilized Arduino core's I2C library.
            3. Pass a reference to the instantiated ``TwoWire`` object to `PinnacleTouchI2C::begin()`.
