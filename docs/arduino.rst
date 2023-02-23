Arduino Support
===============

This library was primarily written for the Arduino IDE. Therefore, it uses the SPI and I2C bus
implementations from the Arduino framework (as provided by various board support -- AKA "Arduino
Cores").

Using a non-default SPI bus
---------------------------

It is possible to use a specific SPI bus on boards that expose more than 1 SPI bus. As noted above,
the ``SPIClass`` interface may vary depending on the targeted board. In general, the procedure is
as follows:

1. Instantiate a ``SPIClass`` object for use with a specific SPI bus.
2. Initialize the SPI bus using ``SPIClass::begin()`` method. The arguments passed to this function
   may differ depending on the Arduino core used (for a certain family of boards). Developers are
   encouraged to identify the use of ``SPIClass::begin()`` as instructed by the utilized Arduino
   core's SPI library.
3. Pass a reference to the instantiated ``SPIClass`` object to `PinnacleTouchSPI::begin()`.

The following code snippet exemplifies these steps:

.. code-block:: cpp

    #include <CirquePinnacle.h>
    #include <SPI.h>

    #define SS_PIN 5
    #define DR_PIN 6

    SPIClass my_spi_obj = SPIClass();

    PinnacleTouch trackpad = PinnacleTouchSPI(DR_PIN, SS_PIN);

    void begin()
    {
      Serial.begin(115200);
      while (!Serial) {
        // some boards need this for the prompts to show immediately after boot
      }

      my_spi_obj.begin(); // refer to Arduino Core's SPI library for more detail

      if (!trackpad.begin(&my_spi_obj)) {
        Serial.println(F("Cirque Pinnacle not responding!"));
        while (true) {} // hold program in infinite loop
      }

      // continue program as usual...
    }


Using a non-default I2C bus
---------------------------

It is possible to use a specific I2C bus on boards that expose more than 1 I2C bus. As noted above,
the ``TwoWire`` interface may vary depending on the targeted board. In general, the procedure is
as follows:

1. Instantiate a ``TwoWire`` object for use with a specific I2C bus.
2. Initialize the I2C bus using ``TwoWire::begin()`` method. The arguments passed to this function
   may differ depending on the Arduino core used (for a certain family of boards). Developers are
   encouraged to identify the use of ``TwoWire::begin()`` as instructed by the utilized Arduino
   core's I2C library.
3. Pass a reference to the instantiated ``TwoWire`` object to `PinnacleTouchI2C::begin()`.

The following code snippet exemplifies these steps:

.. code-block:: cpp

    #include <CirquePinnacle.h>
    #include <Wire.h>

    #define DR_PIN 6

    TwoWire my_i2c_obj = TwoWire();

    PinnacleTouch trackpad = PinnacleTouchI2C(DR_PIN);

    void begin()
    {
      Serial.begin(115200);
      while (!Serial) {
        // some boards need this for the prompts to show immediately after boot
      }

      my_i2c_obj.begin(); // refer to Arduino Core's I2C library for more detail

      if (!trackpad.begin(&my_i2c_obj)) {
        Serial.println(F("Cirque Pinnacle not responding!"));
        while (true) {} // hold program in infinite loop
      }

      // continue program as usual...
    }
