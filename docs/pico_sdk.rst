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
`incorporating_in_user_src`_.

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

2. Configure CMake for your desired build type and specific RP2040-based board

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
3. Build the examples using the CMakeLists.txt file located in the
   CirquePinnacle/examples/pico_sdk directory.

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

In order to use the CirquePinnacle libraries in your RP2040 based project:

1. Make a copy of the CirquePinnacle library in a "lib" directory located in your project's root directory.

   .. code-block:: text

       path/to/my/project/
           lib/
               CirquePinnacle/
           src/
               CMakeLists.txt
               ...

   Alternatively you can add the CirquePinnacle repositories as
   `git submodules <https://git-scm.com/book/en/v2/Git-Tools-Submodules>`_.
2. Include the root CMakeLists.txt file from the CirquePinnacle library in your project's top-level
   CMakeLists.txt file (usually located in the "src" directory). The following snippet
   assumes that your project's "src" directory is on the same level as the previously
   mentioned "lib" directory.

   .. code-block:: cmake

       include(../lib/CirquePinnacle/src/CMakeLists.txt)

3. In the same CMakeLists.txt file (in your project) from step 2, add the CirquePinnacle library into
   the ``target_link_libraries`` configuration:

   .. code-block:: cmake

       target_link_libraries(${CMAKE_PROJECT_NAME}
           # ... Your project's other libraries ...
           CirquePinnacle
       )

   If you are using tinyUSB, this line (or similar) should already exist:

   .. code-block:: cmake

       target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR})

4. Finally, remember to include the necessary CirquePinnacle library's header files in your
   project's source code where applicable.

   .. code-block:: cpp

       #include <CirquePinnacle.h>

   .. note::
       Any of the project's source file(s) that includes the CirquePinnacle library must be a C++ file.
       It is highly recommended to use the ``.cpp`` file extension for such project source files.

Using different pins for the SPI or I2C bus
*******************************************

Initially (without modification), the SPI bus uses the default pins defined in the
Pico SDK repository's `pico-sdk/src/boards/include/boards/\<board_name>.h files
<https://github.com/raspberrypi/pico-sdk/tree/master/src/boards/include/boards>`_.
However, there may be some boards that do not define the necessary pins to use as defaults. This can
be rememdied using either project source code or build-time configuration.

.. warning::
    There is no support for software driven SPI on RP2040 based boards at this time.
    If someone is so inclined to implement this using the Pico SDK's PIO (Programable Input
    Output) feature, please submit an issue or pull request to the
    `CirquePinnacle repository <http://github.com/nCirquePinnacle/CirquePinnacle>`_.

.. note::
    Before deciding what pins to use for the SPI bus, review the
    `GPIO pins' "Function Select Table" in the Pico SDK documentation
    <https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__gpio.html#details>`_.
    There are essentially 2 SPI buses with multiple pin options for each.

Project Source code option
--------------------------

This option is the most reliable and flexible. It involves calling ``SPI.begin()`` and
then passing the ``SPI`` object to ``PinnacleTouchSPI::begin(_SPI *spi_bus)``.

.. code-block:: cpp

    #include <CirquePinnacle.h>
    CirquePinnacle trackpad(7, 8); // pin numbers connected to the trackpad's DR and SS pins (respectively)
    int main()
    {
        // first pull in the namespace for the pre-instantiated `SPI` object
        using namespace cirque_pinnacle_arduino_wrappers;

        // again please review the GPIO pins' "Function Select Table" in the Pico SDK docs
        SPI.begin(spi0, 2, 3, 4); // spi0 or spi1 bus, SCK, TX, RX
        if (!trackpad.begin(&spi)) {
            printf("Radio hardware is not responding!\n");
        }
        // continue with program as normal ...
    }

Build-time configuration option
-------------------------------

To specify the default SPI or I2C pins used at build time, you can use either:

1. Declare these pins in the CMakeLists.txt file

   .. code-block:: cmake

       target_compile_definitions(${CMAKE_PROJECT_NAME}
           PUBLIC PICO_DEFAULT_SPI=0 # can only be 0 or 1 (as in `spi0` or `spi1`)
           PUBLIC PICO_DEFAULT_SPI_SCK_PIN=2 # depends on which SPI bus (0 or 1) is being used
           PUBLIC PICO_DEFAULT_SPI_TX_PIN=3  # depends on which SPI bus (0 or 1) is being used
           PUBLIC PICO_DEFAULT_SPI_RX_PIN=4  # depends on which SPI bus (0 or 1) is being used
           PUBLIC PICO_DEFAULT_I2C=0 # can only be 0 or 1 (as in `i2c0` or `i2c1`)
           PUBLIC PICO_DEFAULT_I2C_SCL_PIN=7 # depends on which I2C bus (0 or 1) is being used
           PUBLIC PICO_DEFAULT_I2C_SDA_PIN=8 # depends on which I2C bus (0 or 1) is being used
       )

2. additional command line arguments

   .. code-block:: shell

       cmake --build . --config Release \
       -DPICO_DEFAULT_SPI=0 \
       -DPICO_DEFAULT_SPI_SCK_PIN=2 \
       -DPICO_DEFAULT_SPI_TX_PIN=3 \
       -DPICO_DEFAULT_SPI_RX_PIN=4 \
       -DPICO_DEFAULT_I2C=0 \
       -DPICO_DEFAULT_I2C_SCL_PIN=7 \
       -DPICO_DEFAULT_I2C_SDA_PIN=8

Using Multiple Cirque Pinnacle Trackpads
****************************************

It is possible to drive multiple Cirque Pinnacle Trackpads on a single board.
To do this each trackpad needs dedicated digital output pins for the DR and SS pins.

If you want to drive each trackpad with a separate SPI bus, then the following example will demonstrate how to do that.

.. code-block:: cpp

    #include <CirquePinnacle.h>
    // Declare the pin numbers connected to the trackpads' DR and SS pins (respectively)
    CirquePinnacle trackpad0(8, 5);   // first trackpad object
    CirquePinnacle trackpad1(14, 13); // second trackpad object
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
            printf("Trackpad0 hardware is not responding!\n");
            return false;
        }
        // first trackpad object initialized successfully
        // specify the the second SPI bus interface and corresponding GPIO pins
        my_spi.begin(spi1, 10, 11, 12); // spi1 bus, SCK, TX, RX
        if (!trackpad1.begin(&my_spi)) {
            printf("Trackpad1 hardware is not responding!\n");
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
