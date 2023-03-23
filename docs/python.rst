Python Binding
==============

This library has bindings for use in CPython on Linux. While it is only meant to function on Linux,
you can still install the python bindings on other platforms which will only yield the stubs-only
package (type hints) for the python bindings.

Installing from Github
**********************

1. Building the bindings from the source code requires a couple dependencies.
   On linux, the CPython's dev headers and CMake are needed.

   .. code-block:: shell
       :caption: on Linux

       sudo apt install python3-dev cmake

2. Clone the repository and initialize the submodule for pybind11.

   .. code-block:: shell

       git clone --recurse-submodules https://github.com/2bndy5/CirquePinnacle.git

   Or, if you already cloned the repository, you can simply ensure the pybind11 submodule is
   initialized in the src folder:

   .. code-block:: shell
       :caption: from the repository's root folder

       git submodule update --init src/pybind11

3. Navigate into the repository folder.

   .. code-block:: shell

       cd CirquePinnacle

4. Build and install the package:

   .. code-block:: shell

       python3 -m pip install . -v

   The optional ``-v`` will use ``pip``\ 's verbosity to show that the process isn't frozen. Otherwise, the
   step labeled "building wheel" may look like it isn't doing anything for a few minutes.

   .. tip::
       :title: Optional ``CMAKE_ARGS`` environment variable
       :collapsible:

       You can customize the build further using an environment variable named ``CMAKE_ARGS`` set to a string of
       space-separated options that get passed to CMake. Supported options include:

       ``-DPINNACLE_SPI_SPEED=13000000``
           The SPI speed can be set with ``-DPINNACLE_SPI_SPEED=xxx`` to lower the default speed/baudrate used on
           the SPI bus. Default value is the maximum 13 MHz.

       ``-DPINNACLE_DRIVER=<utility-folder-name>``
           Use this to change the underlying implementation used for the I2C and SPI busses. Supported options include:

           - ``linux_kernel`` (default) is recommended for best user experience and cross-platform/architecture compatibility.
           - ``bcm2xxx`` is a bit slower and only works on RPi boards (requires ``sudo`` permission to execute).
           - ``mraa`` requires the MRAA library installed.
           - ``pigpio`` requires the PiGPIO library installed (requires ``sudo`` permission to execute).

       ``-DPINNACLE_ANYMEAS_SUPPORT=OFF``
           To reduce the compile size of the CirquePinnacle library, you can use ``-DPINNACLE_ANYMEAS_SUPPORT=OFF``
           when the application won't use the Pinnacle's anymeas mode.

5. Open one of the python examples (located in examples/cpython), change the pin numbers accordingly, and run the example.

   .. seealso::
       The :cpp:expr:`PinnacleTouchSPI::begin(pinnacle_spi_t*)` function is not exposed in the python binding.
       Please review how to specify the :ref:`slaveSelectPin` for Linux platforms.

   .. code-block:: shell

       python3 examples/cpython/relative_mode.py

   .. important::
       Remember that the drivers ``pigpio`` and ``bcm2xxx`` will require ``sudo`` permission.
       This *will* cause problems if using ``sudo`` from a python virtual environment because
       ``sudo`` will invoke the system install of the python interpreter (not the virtual
       environment's install of the python interpreter).

       If you need to use ``sudo`` from within a virtual environment, then you *must* specify the
       path to the virtual environment's install of the python interpreter.

       .. code-block:: shell
           :caption: given that the virtual environment is located in ``~/venv``

           sudo ~/venv/bin/python examples/cpython/relative_mode.py
