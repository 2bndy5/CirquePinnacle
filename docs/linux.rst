Linux Support
=============

This library can be used on any Linux device that exposes GPIO pins.

Automated install script
************************

There is a file named ``install-linux.sh`` that automates the process outlined in :ref:`man_install` section.
To use this script the file permission must be modified (only once):

.. code-block:: shell

    chmod u+x install-linux.sh

Now you can run the script and follow the prompts:

.. code-block:: shell

    ./install-linux.sh

.. _man_install:

Installing from Source
**********************

1. Building the library from the source code requires CMake.

   .. code-block:: shell
       :caption: on Linux

       sudo apt install cmake

2. Clone the repository:

   .. code-block:: shell

       git clone https://github.com/2bndy5/CirquePinnacle.git

3. Navigate into the repository folder and create a build folder. Then navigate into the build folder.

   .. code-block:: shell

       cd CirquePinnacle
       mkdir build && cd build

4. Configure CMake to build the library:

   .. code-block:: shell

       cmake ../src -DPINNACLE_DRIVER=linux_kernel

   The ``-DPINNACLE_DRIVER`` will force the build to use 1 of the supported hardware drivers.
   This can also be specified using an environment variable named ``PINNACLE_DRIVER``.
   With out this specified, CMake will look for required dependencies and use the first driver found.
   Supported options include (in order of precedence):

   - ``bcm2xxx`` is a bit slower and only works on RPi boards (requires ``sudo`` permission to execute).
   - ``pigpio`` requires the PiGPIO library installed (requires ``sudo`` permission to execute).
   - ``mraa`` requires the MRAA library installed.
   - ``linux_kernel`` (default fallback) is recommended for best user experience and
     cross-platform/architecture compatibility.

   .. tip::
       :title: Optional arguments
       :collapsible:

       ``-DPINNACLE_SPI_SPEED=6000000``
           The SPI speed can be set with ``-DPINNACLE_SPI_SPEED=xxx`` to lower the default speed/baudrate used on
           the SPI bus. Default value is the officially recommended 6 MHz; maximum supported is 13 MHz.

       ``-DPINNACLE_ANYMEAS_SUPPORT=OFF``
           To reduce the compile size of the CirquePinnacle library, you can use ``-DPINNACLE_ANYMEAS_SUPPORT=OFF``
           when the application won't use the Pinnacle's anymeas mode.
5. Build and install the library:

   .. code-block:: shell

       make
       sudo make install

Run an example
**************

After the library is installed, open one of the linux examples (located in examples/linux) and change the pin
numbers accordingly. The following steps will use a build folder created in the CirquePinnacle
repository's root folder (as created in step 3 above).

1. First make sure the created build folder is empty.

   .. code-block:: shell
       :caption: :si-icon:`fontawesome/regular/trash-can;pulsing-red`
           Be sure to do this from within the build folder!!!
           :si-icon:`material/sign-yield;pulsing-red`

       rm -r ./*

2. Configure CMake to build the examples:

   .. code-block:: shell

       cmake ../examples/linux -DPINNACLE_DRIVER=linux_kernel

   Again, specify the driver used to build the library (see step 4 above) is recommended because
   some drivers require the built applications to be linked to pre-installed libraries.

   .. tip::
       :title: Optional arguments
       :collapsible:

       ``-DUSE_I2C=ON``
           If using the I2C interface (`PinnacleTouchI2C`), then you can enable this for the examples with
           ``-DUSE_I2C=ON``.

       ``-DUSE_SW_DR=ON``
           If not using a physical GPIO pin for the Data Ready pin, then you can use ``-DUSE_SW_DR=ON`` which
           will automatically make the examples use ``PINNACLE_SW_DR`` value for the ``dataReadyPin`` parameters to
           the `~PinnacleTouchSPI::PinnacleTouchSPI()` and `~PinnacleTouchI2C::PinnacleTouchI2C()` constructors.
           The anymeas_mode example is automatically excluded from the build targets since it requires a hardware
           Data Ready pin.
3. Build the examples:

   .. code-block:: shell

       make
4. Run an example:

   .. code-block:: shell

       ./relative_mode

   Remember that both ``PINNACLE_DRIVER``\ s ``bcm2xxx`` and ``pigpio`` will require ``sudo`` permission.

.. _slaveSelectPin:

SlaveSelect pin
***************

Using the SPI bus' SS pin (Slave Select, aka Chip Select) on a Linux platform is a bit different
from the Arduino platform because the Linux kernel controls the pin during bus transactions.
Therefore, the pin number passed to the `~PinnacleTouchSPI::PinnacleTouchSPI()` constructor should
follow the form ``ab`` where ``a`` is the SPI bus number and ``b`` is the specified bus' SS pin
(often labeled ``CE<b>`` on Raspberry Pi pinout diagrams).

.. csv-table::
    :header: "bus ID","CE number","constructor's ``slaveSelectPin`` value","spidev adapter"
    :widths: 2, 4, 8, 6

    ``0``,``0``,:expr:`0`,``/dev/spidev0.0``
    ``0``,``1``,:expr:`1`,``/dev/spidev0.1``
    ``1``,``0``,:expr:`10`,``/dev/spidev1.0``
    ``1``,``1``,:expr:`11`,``/dev/spidev1.1``
    ``1``,``2``,:expr:`12`,``/dev/spidev1.2``

.. note::
    Support for the auxiliary (AKA secondary) SPI bus (``/dev/spidev1.x``) is only well supported in
    the ``linux_kernel`` driver. Other drivers have their own set of limitations when it comes to
    using ``/dev/spidev1.x``.

Using a non-default I2C bus
***************************

The default I2C bus used is ``/dev/i2c-1``. However, some boards may use a different I2C bus number
as a default. This can be remedied by passing the correct bus number to `cirque_pinnacle_arduino_wrappers::TwoWire::begin()`.

.. code-block:: cpp
    :caption: To use ``/dev/i2c-0`` bus
    :class: annotated-with-numbers
    :emphasize-lines: 7,9

    #include <CirquePinnacle.h>
    #define DR_PIN 25
    PinnacleTouchI2C trackpad(DR_PIN);

    int main() {
        // specify the I2C bus
        cirque_pinnacle_arduino_wrappers::Wire.begin(0); // (1)!

        if (!trackpad.begin(&cirque_pinnacle_arduino_wrappers::Wire)) { // (2)!
            return 1; // failed to initialize the trackpad
        }
        // continue the program as usual ...
    }

.. code-annotations::
    1. Use :expr:`0` for ``/dev/i2c-0``. Default is :expr:`1` for ``/dev/i2c-1``.

       .. seealso:: `cirque_pinnacle_arduino_wrappers::TwoWire::begin()`
    2. Explicitly pass a reference of the `~cirque_pinnacle_arduino_wrappers::TwoWire` object to
       `PinnacleTouchI2C::begin()`.
