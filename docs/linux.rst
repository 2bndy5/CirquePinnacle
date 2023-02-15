Linux Support
=============

This library can be used on any Linux device that exposes GPIO pins.

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

       cmake ../src -D CIRQUE_PINNACLE_DRIVER=linux_kernel
   
   The ``-D CIRQUE_PINNACLE_DRIVER`` will force the build to use 1 of the supported hardware drivers.
   This can also be specified using an environment variable named ``CIRQUE_PINNACLE_DRIVER``.
   With out this specified, CMake will look for required dependencies and use the first driver found.
   Supported options include (in order of precedence):

   - ``bcm2xxx`` is a bit slower and only works on RPi boards (requires ``sudo`` permission to execute).
   - ``pigpio`` requires the PiGPIO library installed (requires ``sudo`` permission to execute).
   - ``mraa`` requires the MRAA library installed (requires ``sudo`` permission to execute).
   - ``linux_kernel`` (default fallback) is recommended for best user experience and
     cross-platform/architecture compatibility.

   Additionally, ``-D PINNACLE_SPI_SPEED=13000000`` can be set to lower the baudrate used on the SPI bus.
   Default value is the maximum 13 MHz.
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
       :caption: Be sure to do this from within the build folder!!!

       rm -r ./*

2. Configure CMake to build the examples:

   .. code-block:: shell

       cmake ../examples/linux -D CIRQUE_PINNACLE_DRIVER=linux_kernel

   Again, specify the driver used to build the library (see step 4 above) is recommended because
   some drivers require the built applications to be linked to ``pthread``.
3. Build the examples:

   .. code-block:: shell

       make
4. Run an example:

   .. code-block:: shell

       ./relative_mode

   Remember that any driver other than ``linux_kernel`` will require ``sudo`` permission.

.. _slaveSelectPin:

SlaveSelect pin
***************

Using the SPI bus' SS pin (Slave Select, aka Chip Select) on a Linux platform is a bit different
from the Arduino platform because the Linux kernel controls the pin during bus transactions.
Therefore, the pin number passed to the `PinnacleTouchSPI` constructor should follow the form
``ab`` where ``a`` is the SPI bus number and ``b`` is the specified bus' SS pin (often labeled
``CE<b>`` on Raspberry Pi pinout diagrams).

.. csv-table::
    :header: "bus ID","CE number","constructor's ``slaveSelectPin`` value","spidev adapter"

    0,0,0,``/dev/spidev0.0``
    0,1,1,``/dev/spidev0.1``
    1,0,10,``/dev/spidev1.0``
    1,1,11,``/dev/spidev1.1``
    1,2,12,``/dev/spidev1.2``
