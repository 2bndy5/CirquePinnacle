Python API
==========

The python bindings use the same API that is publicly exposed in the :doc:`cpp`.
There are a few differences to make the Python API more "pythonic".

Name Casing
-----------

Any functions that use camel casing are exposed. There are also equivalent python functions that
use snake casing.

.. code-block:: python

    trackpad.relative_mode_config()
    # is equivalent to
    trackpad.relativeModeConfig()

A list of exposed functions in python and their exposed C++ equivalents:

.. list-table::
    :header-rows: 1

    * - Python
      - C++
    * - ``PinnacleTouch.available()``
      - `PinnacleTouch::available()`
    * - ``PinnacleTouch.absolute_mode_config()``
      - `PinnacleTouch::absoluteModeConfig()`
    * - ``PinnacleTouch.relative_mode_config()``
      - `PinnacleTouch::relativeModeConfig()`
    * - ``PinnacleTouch.read()``
      - `PinnacleTouch::read()`
    * - ``PinnacleTouch.clear_status_flags()``
      - `PinnacleTouch::clearStatusFlags()`
    * - ``PinnacleTouch.detect_finger_stylus()``
      - `PinnacleTouch::detectFingerStylus()`
    * - ``PinnacleTouch.calibrate()``
      - `PinnacleTouch::calibrate()`
    * - ``PinnacleTouch.set_adc_gain()``
      - `PinnacleTouch::setAdcGain()`
    * - ``PinnacleTouch.tune_edge_sensitivity()``
      - `PinnacleTouch::tuneEdgeSensitivity()`
    * - ``PinnacleTouch.anymeas_mode_config()``
      - `PinnacleTouch::anymeasModeConfig()`
    * - ``PinnacleTouch.measure_adc()``
      - `PinnacleTouch::measureAdc()`
    * - ``PinnacleTouch.start_measure_adc()``
      - `PinnacleTouch::startMeasureAdc()`
    * - ``PinnacleTouch.get_measure_adc()``
      - `PinnacleTouch::getMeasureAdc()`

Properties vs setter and getters
--------------------------------

Some C++ getter and setter functions are expose as python properties.

.. code-block:: python

    trackpad.data_mode = PINNACLE_ABSOLUTE
    # is equivalent to
    trackpad.setDataMode(PINNACLE_ABSOLUTE)

    mode = trackpad.data_mode
    # is equivalent to
    mode = trackpad.getDataMode()


A list of properties and their exposed C++ equivalent setters/getters:

.. list-table::
    :header-rows: 1

    * - Python
      - C++
    * - ``PinnacleTouch.data_mode``
      - + `PinnacleTouch::getDataMode()`
        + `PinnacleTouch::setDataMode()`
    * - ``PinnacleTouch.feed_enable``
      - + `PinnacleTouch::isFeedEnabled()`
        + `PinnacleTouch::feedEnabled()`
    * - ``PinnacleTouch.is_hard_configured`` (read-only)
      - + `PinnacleTouch::isHardConfigured()`
    * - ``PinnacleTouch.allow_sleep``
      - + `PinnacleTouch::isAllowSleep()`
        + `PinnacleTouch::allowSleep()`
    * - ``PinnacleTouch.shutdown`` [note]_
      - + `PinnacleTouch::isShutdown()`
        + `PinnacleTouch::shutdown()`
    * - ``PinnacleTouch.sample_rate``
      - + `PinnacleTouch::getSampleRate()`
        + `PinnacleTouch::setSampleRate()`
    * - ``PinnacleTouch.calibration_matrix``
      - + `PinnacleTouch::getCalibrationMatrix()`
        + `PinnacleTouch::setCalibrationMatrix()`

.. [note]
    .. failure:: Missing

        One exception to this is that there is no function in the python binding for
        `PinnacleTouch::shutdown()` and `PinnacleTouch::isShutdown()`.
        Due to a naming conflict, these methods are only exposed via the
        ``PinnacleTouch.shutdown`` property.


Calibration Matrix
------------------

Setting the calibration matrix is done in python using a :py:class:`list` of integers.
Each integer must still fit within 2 bytes (signed).

.. code-block:: python

    values: List[int] = list(46)  # maximum length is 46 integers

    trackpad.calibration_matrix = values
    # is equivalent to
    trackpad.setCalibrationMatrix(values)

    values = trackpad.calibration_matrix
    # is equivalent to
    values = trackpad.getCalibrationMatrix()

``RelativeReport.buffer``
-------------------------

There is an added read-only property, named ``buffer``, in the :cpp:class:`RelativeReport` class.
This is done in the python binding to expedite the formation of an immutable python buffer
(:py:class:`bytes`). This is helpful when using relative mode data as a mouse HID report buffer.

.. code-block:: python

    report = RelativeReport()
    touchpad.read(report)  # relative mode data saved to `report` object.
    # optionally manipulate the reported data (eg. apply acceleration or axis inversion)
    buf: bytes = report.buffer  # can be used as a mouse HID report

.. hint::
    Both :cpp:class:`RelativeReport` and :cpp:class:`AbsoluteReport` classes in the python
    binding have customized ``__repr__()`` functions to quickly show the data structures' values.

    .. code-block:: python

        >>> report = AbsoluteReport()
        >>> report.x = 64
        >>> report.y = 128
        >>> report.z = 32
        >>> print(report)
        <AbsoluteReport B1: 0 B2: 0 B3: 0 X: 64 Y: 128 Z: 32>

Exposed I2C implementation
--------------------------

Since multiple buses are supported by this library, the underlying class ``TwoWire`` is exposed
that implements the I2C protocol similar to `the Arduino API
<https://www.arduino.cc/reference/en/language/functions/communication/wire/>`_. Typical
applications shouldn't need to use this as most Linux-based SoC boards only have 1 I2C bus exposed
in the GPIO pins.

The I2C bus number can be different depending on how the manufacturer routed the I2C bus
from the SoC. Thus, the :cpp:expr:`PinnacleTouchI2C::begin(pinnacle_i2c_t*)` method is exposed,
so users can specify an I2C bus number that may differ from the default value used (which is :cpp:`1`).
The first Raspberry Pi board exposed ``/dev/i2c-0``, but later iterations changed this to
``/dev/i2c-1``.

.. code-block:: python
    :caption: Using the ``/dev/i2c-0`` bus

    from cirque_pinnacle import PinnacleTouchI2C, TwoWire

    i2c_bus = TwoWire()
    i2c_bus.begin(0)  # specify the bus number here
    trackpad = PinnacleTouchI2C(DR_PIN)
    ok = trackpad.begin(i2c_bus)  # feed the custom I2C bus obj here
    if not ok:
        raise OSError("failed to find the trackpad")

.. py:module:: cirque_pinnacle

.. |stop_param_ignored| replace:: This parameter's value is ignored because repeated stop conditions and
    behaving like a I2C slave device is not supported in this implementation. A stop
    condition is always sent after

.. py:class:: TwoWire

    The actual implementation of this class depends on what ``PINNACLE_DRIVER`` was specified when
    the python binding was installed. By default, the python binding uses ``linux_kernel``. Review
    the :doc:`Python binding install instructions <../python>` for how to specify the
    ``PINNACLE_DRIVER`` to use.

    .. failure:: Missing features

        Interrupt Service Routines (ISR), acting as a slave device, and timeouts are not supported
        in this implementation.

    .. py:method:: begin(busNumber: int = 1) -> None

        Specify the ``busNumber`` as indicated in the ``/dev/i2c-<x>``.

        :param busNumber: The I2C bus number as identified by the directory listing in
            ``/dev/i2c-*``. For ``/dev/i2c-1``, this parameter's value should be :python:`1`.

            .. info:: Difference with ``mraa`` driver
                :collapsible:

                If using the ``mraa`` driver, then this number is not guaranteed to coincide with the
                actual I2C bus number (``/dev/i2c-<x>``). See the `MRAA source code
                <https://github.com/eclipse/mraa/tree/master/src>`_ for your platform to determine
                what number to use for which I2C bus.

                For compatibility reasons, this parameter defaults to :python:`0` when using the
                ``mraa`` driver.

    .. py:method:: end()

        Release the specified I2C bus.

    .. py:method:: beginTransmission(address: int) -> None

        Begin preparing for a buffered write operation to the specified I2C device's address.

        :param address: The I2C slave device's address.

    .. py:method:: write(data: int) -> int

        Add data to the buffer for a write operation. Data is not actually sent until
        :py:meth:`endTransmission()` is called.

        .. warning::
            This implementation uses an internal buffer that allocates 32 bytes. If more than 32
            bytes are added to the internal buffer, then all bytes in excess are dropped.

        :param data: A single byte to add to the internal buffer.

        :returns: The amount of data (in bytes) added. This should always be 1.

    .. py:method:: endTransmission(sendStop: int = 1) -> int

        Perform a buffered write operation over the I2C bus.

        :param sendStop: |stop_param_ignored| the buffered data is written.

        :returns: The amount of data (in bytes) written.

    .. py:method:: requestFrom(address: int, quantity: int, sendStop: int = 1) -> int

        Read a number of bytes from the specified I2C address.

        :param address: The I2C slave device's address.
        :param quantity: The number of bytes to read.

            .. warning::
                This implementation uses an internal buffer that allocates 32 bytes. If more than 32
                bytes are requested, then only 32 bytes are read.
        :param sendStop: |stop_param_ignored| the received data is stored in the internal buffer.

        :returns: The amount of data (in bytes) read. This should always be the ``quantity``
          specified.

    .. py:method:: available() -> int

        Get the number of bytes ready to read.

        .. warning::
            This should only be used after calling :py:meth:`requestFrom()`. Otherwise, the data
            returned may be about the data passed to :py:meth:`write()` (which uses the
            same internal buffer).

        :returns: The amount of data (in bytes) ready read to read from the internal buffer.

    .. py:method:: read() -> int

        Read a byte of data from the internal buffer.

        .. error::
            Make sure to call :py:meth:`requestFrom()` before using this function. Otherwise,
            the data returned may be from the data passed to :py:meth:`write()` (which uses the
            same internal buffer).

        :returns: A single byte. If there is no more data to read, then the value :python:`-1`
            is returned.

            .. hint:: Use :py:meth:`available()` to determine if there is more data to read.
