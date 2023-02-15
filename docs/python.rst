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

3. Navigate into the repository folder.

   .. code-block:: shell

       cd CirquePinnacle

4. Build and install the package:

   .. code-block:: shell

       python3 -m pip install . -v

   The optional ``-v`` will use ``pip``\ 's verbosity to show that the process isn't frozen. Otherwise, the
   step labeled "building wheel" may look like it isn't doing anything for a few minutes.

   You can customize the build further using an environment variable named ``CMAKE_ARGS``. Supported options
   include:

   - ``PINNACLE_SPI_SPEED`` can be set to lower the baudrate used on the SPI bus. Default value is the maximum
     13 MHz.
   - ``CIRQUE_PINNACLE_DRIVER`` can be used to change the underlying implementation used for the I2C and SPI busses.
     Supported options include:

     - ``linux_kernel`` (default) is recommended for best user experience and cross-platform/architecture compatibility.
     - ``bcm2xxx`` is a bit slower and only works on RPi boards (requires ``sudo`` permission to execute).
     - ``mraa`` requires the MRAA library installed (requires ``sudo`` permission to execute).
     - ``pigpio`` requires the PiGPIO library installed (requires ``sudo`` permission to execute).

5. Open one of the python examples (located in examples/cpython), change the pin numbers accordingly, and run the example.

   .. seealso::
       Review how to specify the :ref:`slaveSelectPin` for Linux platforms.

   .. code-block:: shell

       python3 examples/cpython/relative_mode.py

   .. important::
       Remember that any driver other than ``linux_kernel`` will require ``sudo`` permission.
       This *will* cause problems if using ``sudo`` from a python virtual environment because ``sudo``
       will invoke the system install of the python interpreter (not the virtual environment's
       install of the python interpreter).

       If you need to use ``sudo`` from within a virtual environment, then you must specify the
       path to the virtual environment's install of the python interpreter.

       .. code-block:: shell
           :caption: given that the virtual environment is located in ``~/venv``
           
           sudo ~/venv/bin/python example/cpython/relative_mode.py
