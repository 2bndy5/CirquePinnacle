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

    trackpad.data_mode = PINNACLE_ABSOLUTE.value
    # is equivalent to
    trackpad.setDataMode(PINNACLE_ABSOLUTE.value)

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

        One exception to this is there is no function in the python binding for
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

Overloaded ``read()``
----------------------

There is an added ``read()`` overload method in the python binding to expedite the formation of
a python buffer (:py:class:`bytearray`) from the data reported by the Pinnacle ASIC.
This is helpful when using relative mode data as a mouse HID report buffer.

.. note::
    This overloaded method only applies to `PINNACLE_ABSOLUTE` or `PINNACLE_RELATIVE` modes.
    It returns an empty buffer if the data mode is set to `PINNACLE_ANYMEAS` mode.

.. code-block:: python

    report: bytearray = touchpad.read()
