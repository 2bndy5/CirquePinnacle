RPi Pico SDK Support
====================

Just to be very clear and concise: The following instructions are
**not required to use the Arduino IDE** with any RP2040-based boards.
These instructions pertain to using only
`Raspberry Pi's Pico SDK <https://github.com/raspberrypi/pico-sdk>`_
for RP2040 based boards.

Prerequisite
************

Follow Raspberry Pi's
`"Getting Started with Pico" document <https://rptl.io/pico-get-started>`_ to
setup a proper development environment on your host PC (the machine that
will build your project). Notice that the setup instructions are a little
different for building on a Windows PC.

Either set an environment variable named `PICO_SDK_PATH` that points to your
local clone of the pico-sdk or put the pico-sdk next to the CirquePinnacle folder or
next to the folder containing your project using the CirquePinnacle library:

.. code-block:: text

    path/to/github/repos/
        pico-sdk/
        CirquePinnacle/
        my_pinnacle_project/

Alternatively, the CirquePinnacle repository can be included into your project's "lib"
folder as copies or git submodules. For more detail, see the below instructions to
:ref:`incorporating_in_user_src`.

Building the library examples for the Pico SDK
**********************************************

Each example imports a set of constants (``DR_PIN``, ``SS_PIN``) from the
examples/pico_sdk/defaultPins.h file. Default values for SPI pins (SCK, MOSI, MISO)
and I2C pins (SDA & SCL) come from the Pico SDK repository's
`pico-sdk/src/boards/include/boards/\<board_name>.h files
<https://github.com/raspberrypi/pico-sdk/tree/master/src/boards/include/boards>`_.

1. Create a "build" directory in the CirquePinnacle repository's root directory and
   navigate to it:

   .. code-block:: shell

       cd CirquePinnacle
       mkdir build
       cd build

2. Configure CMake for your desired build type and specific a RP2040-based board. This step uses the CMakeLists.txt file located in the
   CirquePinnacle/examples/pico_sdk directory.

   .. code-block:: shell
       :caption: on Linux

       cmake ../examples/pico_sdk -DCMAKE_BUILD_TYPE=Release -DPICO_BOARD=pico

   Or if building on a Windows PC:

   .. code-block:: shell
       :caption: on Windows

       cmake -G "NMake Makefiles" ../examples/pico_sdk -DCMAKE_BUILD_TYPE=Release -DPICO_BOARD=pico

   The supported RP2040-based boards are listed in header files in the Pico SDK
   repository's `pico-sdk/src/boards/include/boards/\<board_name>.h files
   <https://github.com/raspberrypi/pico-sdk/tree/master/src/boards/include/boards>`_.

   If the ``-DPICO_BOARD`` option is not specified, then the Pico SDK will default to building for
   the Raspberry Pi Pico board.

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

       ``-DPINNACLE_ANYMEAS_SUPPORT=OFF``
           To reduce the compile size of the CirquePinnacle library, you can use ``-DPINNACLE_ANYMEAS_SUPPORT=OFF``
           when the application won't use the Pinnacle's anymeas mode. This option is not specific to the examples,
           rather it can be specified in any Pico-SDK project that uses the CirquePinnacle library.

       ``-DPINNACLE_SPI_SPEED=6000000``
           The SPI speed can be set with ``-DPINNACLE_SPI_SPEED=xxx`` to lower the default speed/baudrate used on
           the SPI bus. Default value is the officially recommended 6 MHz; maximum supported is 13 MHz. This option
           is not specific to the examples, rather it can be specified in any Pico-SDK project that uses the
           CirquePinnacle library.

3. Build the examples.

   .. code-block:: shell

       cmake --build . --config Release

   Notice we specified the build type again using the ``--config`` option.

   .. note::
       note If you see an error stating "'PICO_DEFAULT_SPI_SCK_PIN' was not declared in this scope",
       then it means the board you selected with the ``-DPICO_BOARD`` option (in step 2) does not have a
       default set of SPI pins defined for it. To workaround this error, see the below instructions to
       use different pins for the SPI bus.

   .. warning::
       If doing consecutive build attempts, it is strongly encouraged to delete all files in the build
       directory before re-attempting to build the project.

.. _incorporating_in_user_src:

Incorporating CirquePinnacle library into your project
******************************************************

In order to use the CirquePinnacle library in your RP2040 based project:

1. Make a copy of the CirquePinnacle library in a "lib" directory located in your project's root directory.

   .. code-block:: text

       path/to/my/project/
           lib/
               CirquePinnacle/
           src/
               CMakeLists.txt
               ...

   Alternatively, you can add the CirquePinnacle repository as a
   `git submodule <https://git-scm.com/book/en/v2/Git-Tools-Submodules>`_.
2. Include the root CMakeLists.txt file from the CirquePinnacle library in your project's top-level
   CMakeLists.txt file (usually located in the "src" directory) *after* invoking the
   ``pico_sdk_init()`` function. The following snippet assumes that your project's "src" directory
   is on the same level as the previously mentioned "lib" directory.

   .. code-block:: cmake
       :emphasize-lines: 10


       # Pull in SDK (must be before `project()`)
       include(../lib/CirquePinnacle/src/cmake/pico_sdk_import.cmake)

       project(${projectName} C CXX ASM)

       # Initialize the Pico SDK
       pico_sdk_init()

       # must be done after `pico_sdk_init()`
       include(../lib/CirquePinnacle/src/CMakeLists.txt)

3. In the same CMakeLists.txt file (in your project) from step 2, add the CirquePinnacle library into
   the ``target_link_libraries`` configuration:

   .. code-block:: cmake

       target_link_libraries(${targetName}
           # ... Your project's other libraries ...
           CirquePinnacle
       )

   If you are using tinyUSB, this line (or similar) should already exist:

   .. code-block:: cmake

       target_include_directories(${targetName} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

4. Finally, remember to include the necessary CirquePinnacle library's header files in your
   project's source code where applicable.

   .. code-block:: cpp

       #include <CirquePinnacle.h>

   .. note::
       Any of the project's source file(s) that includes the CirquePinnacle library must be a C++ file.
       It is highly recommended to use the ``.cpp`` file extension for such project source files.

Using different pins for the SPI or I2C bus
*******************************************

Initially (without modification), the SPI and I2C bus uses the default pins defined in the
Pico SDK repository's `pico-sdk/src/boards/include/boards/\<board_name>.h files
<https://github.com/raspberrypi/pico-sdk/tree/master/src/boards/include/boards>`_.
However, there may be some boards that do not define the necessary pins to use as defaults.
This can be remedied using either the :ref:`pico-sdk-src-opt` or the :ref:`pico-sdk-cfg-opt`.

.. warning::
    There is no support for software-driven (AKA bit-banged) SPI on RP2040 based boards at this time.
    If someone is so inclined to implement this using the Pico SDK's PIO (Programable Input
    Output) feature, please submit an issue or pull request to the
    `CirquePinnacle repository <http://github.com/2bndy5/CirquePinnacle>`_.

.. note::
    Before deciding what pins to use for the SPI bus, review the
    `GPIO pins' "Function Select Table" in the Pico SDK documentation
    <https://www.raspberrypi.com/documentation/pico-sdk/hardware.html#autotoc_md0>`_.
    There are essentially 2 SPI buses with multiple pin options for each.

.. _pico-sdk-src-opt:

Project Source code option
--------------------------

.. md-tab-set::

    .. md-tab-item:: SPI

        This option is the most reliable and flexible. It involves calling ``SPI.begin()`` and
        then passing the ``SPI`` object to
        :cpp:expr:`PinnacleTouchSPI::begin(pinnacle_spi_t *spi_bus)`.

        .. code-block:: cpp
            :emphasize-lines: 7,9

            #include <CirquePinnacle.h>
            PinnacleTouchSPI trackpad(DR_PIN, SS_PIN);
            int main()
            {
                // Using the pre-instantiated `SPI` object
                // Again, please review the GPIO pins' "Function Select Table" in the Pico SDK docs
                cirque_pinnacle_arduino_wrappers::SPI.begin(spi0, 2, 3, 4); // spi0 or spi1 bus, SCK, TX, RX

                if (!trackpad.begin(&cirque_pinnacle_arduino_wrappers::SPI)) {
                    printf("Cirque Pinnacle not responding!\n");
                }
                // continue with program as normal ...
            }

    .. md-tab-item:: I2C

        This option is the most reliable and flexible. It involves calling ``Wire.begin()`` and
        then passing the ``Wire`` object to
        :cpp:expr:`PinnacleTouchI2C::begin(pinnacle_i2c_t *i2c_bus)`.

        .. code-block:: cpp
            :emphasize-lines: 7,9

            #include <CirquePinnacle.h>
            PinnacleTouchI2C trackpad(DR_PIN);
            int main()
            {
                // Using the pre-instantiated `Wire` object.
                // Again, please review the GPIO pins' "Function Select Table" in the Pico SDK docs
                cirque_pinnacle_arduino_wrappers::Wire.begin(i2c0, 4, 5); // i2c0 or i2c1 bus, SDA, SCL

                if (!trackpad.begin(&cirque_pinnacle_arduino_wrappers::Wire)) {
                    printf("Cirque Pinnacle not responding!\n");
                }
                // continue with program as normal ...
            }

.. _pico-sdk-cfg-opt:

Build-time configuration option
-------------------------------

To specify the default SPI or I2C pins used at build time, you can declare them in the CMakeLists.txt file.

.. md-tab-set::

    .. md-tab-item:: SPI

        .. code-block:: cmake

            target_compile_definitions(${CMAKE_PROJECT_NAME}
                PUBLIC PICO_DEFAULT_SPI=0 # can only be 0 or 1 (as in `spi0` or `spi1`)
                PUBLIC PICO_DEFAULT_SPI_SCK_PIN=2 # depends on which SPI bus (0 or 1) is being used
                PUBLIC PICO_DEFAULT_SPI_TX_PIN=3  # depends on which SPI bus (0 or 1) is being used
                PUBLIC PICO_DEFAULT_SPI_RX_PIN=4  # depends on which SPI bus (0 or 1) is being used
            )

    .. md-tab-item:: I2C

        .. code-block:: cmake

            target_compile_definitions(${CMAKE_PROJECT_NAME}
                PUBLIC PICO_DEFAULT_I2C=0 # can only be 0 or 1 (as in `i2c0` or `i2c1`)
                PUBLIC PICO_DEFAULT_I2C_SCL_PIN=2 # depends on which I2C bus (0 or 1) is being used
                PUBLIC PICO_DEFAULT_I2C_SDA_PIN=3 # depends on which I2C bus (0 or 1) is being used
            )

Using Multiple Cirque Pinnacle Trackpads
****************************************

It is possible to drive multiple Cirque Pinnacle Trackpads on a single board.
To do this, each trackpad needs dedicated digital output pins for the DR pin (and SS pin for SPI).

If you want to drive each trackpad with a separate SPI or I2C bus, then the following example will
demonstrate how to do that.

.. md-tab-set::

    .. md-tab-item:: SPI

        .. code-block:: cpp
            :emphasize-lines: 12,24-25

            #include <CirquePinnacle.h>

            // Declare the pin numbers connected to the trackpads' DR and SS pins (respectively)
            PinnacleTouchSPI trackpad0(DR_PIN_0, SS_PIN_0); // first trackpad object
            PinnacleTouchSPI trackpad1(DR_PIN_1, SS_PIN_1); // second trackpad object

            // By default, one SPI bus instance is created by the CirquePinnacle lib. We'll use this
            // default instance of the `spi0` interface for our first trackpad, but we want a
            // different SPI bus for the second trackpad.
            //
            // So, here we declare a second SPI bus instance:
            cirque_pinnacle_arduino_wrappers::SPIClass my_spi; // we specify the `spi1` bus interface below

            bool setupTrackpads()
            {
                // Initialize the first trackpad using the default SPI instance
                if (!trackpad0.begin()) {
                    printf("trackpad0 hardware is not responding!\n");
                    return false;
                }
                // first trackpad object initialized successfully

                // specify the the second SPI bus interface and corresponding GPIO pins
                my_spi.begin(spi1, 26, 27, 28); // spi1 bus, SCK, TX, RX
                if (!trackpad1.begin(&my_spi)) {
                    printf("trackpad1 hardware is not responding!\n");
                    return false;
                }
                // second trackpad object initialized successfully
                return true;
            }

            int main()
            {
                stdio_init_all(); // init necessary IO for the RP2040
                while (!setupTrackpads()) { // if either trackpadX.begin() failed
                    sleep_ms(1000); // add 1 second delay for console readability
                    // hold program in infinite attempts to initialize the trackpads
                }
                // continue with program as normal ...
            }

    .. md-tab-item:: I2C

        .. code-block:: cpp
            :emphasize-lines: 12,24-25

            #include <CirquePinnacle.h>

            // Declare the pin numbers connected to the trackpads' DR and SS pins (respectively)
            PinnacleTouchI2C trackpad0(DR_PIN_0); // first trackpad object
            PinnacleTouchI2C trackpad1(DR_PIN_1); // second trackpad object

            // By default, one I2C bus instance is created by the CirquePinnacle lib. We'll use this
            // default instance of the `i2c0` interface for our first trackpad, but we want a
            // different I2C bus for the second trackpad.
            //
            // So, here we declare a second I2C bus instance:
            cirque_pinnacle_arduino_wrappers::TwoWire my_i2c; // we specify the `i2c1` bus interface below

            bool setupTrackpads()
            {
                // Initialize the first trackpad using the default I2C instance
                if (!trackpad0.begin()) {
                    printf("trackpad0 hardware is not responding!\n");
                    return false;
                }
                // first trackpad object initialized successfully

                // specify the the second I2C bus interface and corresponding GPIO pins
                my_i2c.begin(i2c1, 2, 3); // i2c1 bus, SDA, SCL
                if (!trackpad1.begin(&my_i2c)) {
                    printf("trackpad1 hardware is not responding!\n");
                    return false;
                }
                // second trackpad object initialized successfully
                return true;
            }

            int main()
            {
                stdio_init_all(); // init necessary IO for the RP2040
                while (!setupTrackpads()) { // if either trackpadX.begin() failed
                    sleep_ms(1000); // add 1 second delay for console readability
                    // hold program in infinite attempts to initialize the trackpads
                }
                // continue with program as normal ...
            }
