
PinnacleTouch API
==================

Accepted Constants
------------------

Data Modes
***********
   Allowed symbols for configuring the Pinanacle ASIC's data reporting/measurements.

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_RELATIVE=0

   Alias symbol for specifying Relative mode (AKA Mouse mode).

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_ANYMEAS=1

   Alias symbol for specifying "AnyMeas" mode (raw ADC measurement)

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_ABSOLUTE=2

   Alias symbol for specifying Absolute mode (axis positions)

AnyMeas mode Gain
******************

   Allowed ADC gain configurations of AnyMeas mode. The percentages defined here are approximate
   values.

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_GAIN_100

   around 100% gain

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_GAIN_133

   around 133% gain

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_GAIN_166

   around 166% gain

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_GAIN_200

   around 200% gain


AnyMeas mode Frequencies
************************

   Allowed frequency configurations of AnyMeas mode. The frequencies defined here are
   approximated based on an aperture width of 500 nanoseconds. If the ``apertureWidth``
   parameter to :cpp:func:`~PinnacleTouch::anyMeasModeConfig()` specified is less than 500 nanoseconds, then the
   frequency will be larger than what is described here (& vice versa).

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_FREQ_0

   frequency around 500,000Hz

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_FREQ_1

   frequency around 444,444Hz

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_FREQ_2

   frequency around 400,000Hz

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_FREQ_3

   frequency around 363,636Hz

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_FREQ_4

   frequency around 333,333Hz

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_FREQ_5

   frequency around 307,692Hz

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_FREQ_6

   frequency around 267,000Hz

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_FREQ_7

   frequency around 235,000Hz


AnyMeas mode Muxing
*******************

   Allowed muxing gate polarity and reference capacitor configurations of AnyMeas mode.
   Combining these values (with ``+`` operator) is allowed.

   .. note:: The sign of the measurements taken in AnyMeas mode is inverted depending on which
      muxing gate is specified (when specifying an individual gate polarity).

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_MUX_REF1

   enables a builtin capacitor (~0.5pF). See note in :cpp:func:`~PinnacleTouch::measureADC()`

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_MUX_REF0

   enables a builtin capacitor (~0.25pF). See note in :cpp:func:`~PinnacleTouch::measureADC()`

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_MUX_PNP

   enable PNP sense line

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_MUX_NPN

   enable NPN sense line


AnyMeas mode Control
********************

   These constants control the number of measurements performed in :cpp:func:`~PinnacleTouch::measureADC()`.
   The number of measurements can range [0, 63].

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_CRTL_REPEAT

   required for more than 1 measurement

.. cpp:var:: uint8_t PinnacleTouch::PINNACLE_CRTL_PWR_IDLE

   triggers low power mode (sleep) after completing measurements


PinnacleTouch
-------------

Constructor
*************************

.. cpp:class:: PinnacleTouch

   :param int dataReadyPin: The input pin connected to the Pinnacle ASIC's "Data
      Ready" pin. If this parameter is not specified, then the SW_DR (software data ready) flag
      of the STATUS register is used to detirmine if the data being reported is new.

DataMode
*************************

.. cpp:function:: void PinnacleTouch::setDataMode(uint8_t mode)

   Valid input values are :cpp:var:`~PinnacleTouch::PINNACLE_RELATIVE` for
   relative/mouse mode, :cpp:var:`~PinnacleTouch::PINNACLE_ABSOLUTE` for
   absolute positioning mode, or :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS`
   (referred to as "AnyMeas" in specification sheets) mode for reading ADC values.

.. cpp:function:: void PinnacleTouch::getDataMode()

   :Returns:

      - ``0`` for Relative mode (AKA mouse mode)
      - ``1`` for AnyMeas mode (raw ADC measurements)
      - ``2`` for Absolute mode (X & Y axis positions)

   .. important:: When switching from :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS` to
      :cpp:var:`~PinnacleTouch::PINNACLE_RELATIVE` or
      :cpp:var:`~PinnacleTouch::PINNACLE_ABSOLUTE` all configurations are reset, and
      must be re-configured by using  :cpp:func:`~PinnacleTouch::absoluteModeConfig()` or :cpp:func:`~PinnacleTouch::relativeModeConfig()`.

Relative or Absolute mode
*************************

feedEnabled
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::feedEnabled(bool isEnabled)

   This function only applies to :cpp:var:`~PinnacleTouch::PINNACLE_RELATIVE`
   or :cpp:var:`~PinnacleTouch::PINNACLE_ABSOLUTE` mode, otherwise if :cpp:func:`~PinnacleTouch::setDataMode()` is set to
   :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS`, then this function will do nothing.

.. cpp:function:: bool PinnacleTouch::isFeedEnabled()

   :Returns: The setting configured by :cpp:func:`~PinnacleTouch::feedEnabled()`

isHardConfigured()
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: bool PinnacleTouch::isHardConfigured()

   See note about product labeling in `Model Labeling Scheme <index.html#cc>`_. (read only)

   :Returns:
      ``true`` if a 470K ohm resistor is populated at the junction labeled "R4"

relativeModeConfig()
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::relativeModeConfig(bool rotate90, bool glideExtend, bool secondaryTap, bool allTaps, bool intellimouse)

   This function only applies to :cpp:var:`~PinnacleTouch::PINNACLE_RELATIVE`
   mode, otherwise if :cpp:func:`~PinnacleTouch::setDataMode()` is set to
   :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS` or
   :cpp:var:`~PinnacleTouch::PINNACLE_ABSOLUTE`, then this function does nothing.

   :param bool rotate90: Specifies if the axis data is altered for 90 degree rotation before
      reporting it (essentially swaps the axis data). Default is ``false``.
   :param bool glideExtend: A patended feature that allows the user to glide their finger off
      the edge of the sensor and continue gesture with the touch event. Default is ``true``.
      This feature is only available if :cpp:func:`~PinnacleTouch::isHardConfigured()` is ``false``.
   :param bool secondaryTap: Specifies if tapping in the top-left corner (depending on
      orientation) triggers the secondary button data. Defaults to ``true``. This feature is
      only available if :cpp:func:`~PinnacleTouch::isHardConfigured()` is ``false``.
   :param bool allTaps: Specifies if all taps should be reported (``true``) or not
      (``false``). Default is ``true``. This affects ``secondaryTap`` option as well. The
      primary button (left mouse button) is emulated with a tap.
   :param bool intellimouse: Specifies if the data reported includes a byte about scroll data.
      Default is ``false``. Because this flag is specific to scroll data, this feature is only
      available if :cpp:func:`~PinnacleTouch::isHardConfigured()` is ``false``.

absoluteModeConfig()
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::absoluteModeConfig(uint8_t zIdleCount, bool invertX, bool invertY)

   This function only applies to :cpp:var:`~PinnacleTouch::PINNACLE_ABSOLUTE`
   mode, otherwise if :cpp:func:`~PinnacleTouch::setDataMode()` is set to
   :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS` or
   :cpp:var:`~PinnacleTouch::PINNACLE_RELATIVE`, then this function does nothing.

   :param int zIdleCount: Specifies the number of empty packets (x-axis, y-axis, and z-axis
      are ``0``) reported (every 10 milliseconds) when there is no touch detected. Defaults
      to 30. This number is clamped to range [0, 255].
   :param bool invertX: Specifies if the x-axis data is to be inverted before reporting it.
      Default is ``false``.
   :param bool invertY: Specifies if the y-axis data is to be inverted before reporting it.
      Default is ``false``.

reportAbsolute()
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: AbsoluteReport PinnacleTouch::reportAbsolute(bool onlyNew)

   This function only applies to :cpp:var:`~PinnacleTouch::PINNACLE_RELATIVE`
   or :cpp:var:`~PinnacleTouch::PINNACLE_ABSOLUTE` mode, otherwise if :cpp:func:`~PinnacleTouch::setDataMode()` is set to
   :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS`, then this function returns ``NULL`` and does nothing.

   :param bool onlyNew: This parameter can be used to ensure the data reported is only new
      data. Otherwise the data returned can be either old data or new data. If the ``dr_pin``
      parameter is specified upon instantiation, then the specified input pin is used to
      detect if the data is new. Otherwise the SW_DR flag in the STATUS register is used to
      detirmine if the data is new.

   :Returns: ``NULL`` if  the ``only_new`` parameter is set ``true`` and there is no new data to
      report. Otherwise, a ``struct`` of parameters that describe the (touch or
      button) event.

   .. |LessEq| unicode:: U+2264

clearFlags()
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::clearFlags()

AllowSleep
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::setAllowSleep(bool isEnabled)

   Set this attribute to ``true`` if you want the Pinnacle ASIC to enter sleep (low power)
   mode after about 5 seconds of inactivity (does not apply to AnyMeas mode). While the touch
   controller is in sleep mode, if a touch event or button press is detected, the Pinnacle
   ASIC will take about 300 milliseconds to wake up (does not include handling the touch event
   or button press data).

.. cpp:function:: bool PinnacleTouch::getAllowSleep()

   :Returns: The setting configured by :cpp:func:`~PinnacleTouch::setAllowSleep()`

shutdown
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::shutdown(bool isOff)

   ``true`` means powered down (AKA standby mode), and ``false`` means not powered down
   (Active, Idle, or Sleep mode).

   .. note:: The ASIC will take about 300 milliseconds to complete the transition
      from powered down mode to active mode. No touch events or button presses will be
      monitored while powered down.

.. cpp:function:: bool PinnacleTouch::isShutdown()

   :Returns: The setting configured by :cpp:func:`~PinnacleTouch::shutdown()`

SampleRate
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::setSampleRate(uint16_t value)

   Valid values are ``100``, ``80``, ``60``, ``40``, ``20``, ``10``. Any other input values
   automatically set the sample rate to 100 sps (samples per second). Optionally, ``200`` and
   ``300`` sps can be specified, but using these values automatically disables palm (referred
   to as "NERD" in the specification sheet) and noise compensations. These higher values are
   meant for using a stylus with a 2mm diameter tip, while the values less than 200 are meant
   for a finger or stylus with a 5.25mm diameter tip.

   This function only applies to :cpp:var:`~PinnacleTouch::PINNACLE_RELATIVE`
   or :cpp:var:`~PinnacleTouch::PINNACLE_ABSOLUTE` mode, otherwise if :cpp:func:`~PinnacleTouch::setDataMode()` is set to
   :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS`, then this function will do nothing.

.. cpp:function:: bool PinnacleTouch::getSampleRate()

   :Returns: The setting configured by :cpp:func:`~PinnacleTouch::setSampleRate()`

detectFingerStylus()
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::detectFingerStylus(bool enableFinger, bool enableStylus, uint16_t sampleRate)

   :param bool enableFinger: ``true`` enables the Pinnacle ASIC's measurements to
      detect if the touch event was caused by a finger or 5.25mm stylus. ``false`` disables
      this feature. Default is ``true``.
   :param bool enableStylus: ``true`` enables the Pinnacle ASIC's measurements to
      detect if the touch event was caused by a 2mm stylus. ``false`` disables this
      feature. Default is ``true``.
   :param int sampleRate: See the :cpp:func:`~PinnacleTouch::setSampleRate()` attribute as this parameter manipulates that
      attribute.

   .. tip:: Consider adjusting the ADC matrix's gain to enhance performance/results using
      :cpp:func:`~PinnacleTouch::setADCGain()`

calibrate()
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::calibrate(bool run, bool tap, bool trackError, bool nerd, bool background)

   This function only applies to :cpp:var:`~PinnacleTouch::PINNACLE_RELATIVE`
   or :cpp:var:`~PinnacleTouch::PINNACLE_ABSOLUTE` mode, otherwise if :cpp:func:`~PinnacleTouch::setDataMode()` is set to
   :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS`, then this function will do nothing.

   :param bool run: If ``true``, this function forces a calibration of the sensor. If ``false``,
      this function just writes the following parameters to the Pinnacle ASIC's "CalConfig1"
      register. This parameter is required while the rest are optional keyword parameters.
   :param bool tap: Enable dynamic tap compensation? Default is ``true``.
   :param bool trackError: Enable dynamic track error compensation? Default is ``true``.
   :param bool nerd: Enable dynamic NERD compensation? Default is ``true``. This parameter has
      something to do with palm detection/compensation.
   :param bool background: Enable dynamic background compensation? Default is ``true``.

   .. note:: According to the datasheet, calibration of the sensor takes about 100
      milliseconds. This function will block until calibration is complete (if ``run`` is
      ``true``). It is recommended for typical applications to leave all optional parameters
      in their default states.

CalibrationMatrix
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::setCalibrationMatrix(int16_t* matrix)

   This matrix is not applicable in AnyMeas mode. Use this attribute to compare a prior
   compensation matrix with a new matrix that was either loaded manually by setting this
   attribute to an ``array`` of 46 signed 16-bit (short) integers or created internally by calling
   :cpp:func:`~PinnacleTouch::calibrate()` with the ``run`` parameter as ``true``.

   .. note:: A paraphrased note from Cirque's Application Note on Comparing compensation
      matrices:

      If any 16-bit values are above 20K (absolute), it generally indicates a problem with
      the sensor. If no values exceed 20K, proceed with the data comparison. Compare each
      16-bit value in one matrix to the corresponding 16-bit value in the other matrix. If
      the difference between the two values is greater than 500 (absolute), it indicates a
      change in the environment. Either an object was on the sensor during calibration, or
      the surrounding conditions (temperature, humidity, or noise level) have changed. One
      strategy is to force another calibration and compare again, if the values continue to
      differ by 500, determine whether to use the new data or a previous set of stored data.
      Another strategy is to average any two values that differ by more than 500 and write
      this new matrix, with the average values, back into Pinnacle ASIC.

.. cpp:function:: void PinnacleTouch::getCalibrationMatrix()

   :returns:
      The setting configured by :cpp:func:`~PinnacleTouch::setCalibrationMatrix()` or creted internally by :cpp:func:`~PinnacleTouch::calibrate()` (or after a "power-on-reset" condition.

setADCGain()
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::setADCGain(uint8_t sensitivity)

   (does not apply to AnyMeas mode).

   :param int sensitivity: This int specifies how sensitive the ADC (Analog to Digital
      Converter) component is. ``0`` means most sensitive, and ``3`` means least sensitive.
      A value outside this range will default to ``0``.

   .. tip:: The official example code from Cirque for a curved overlay uses a value of ``1``.

tuneEdgeSensitivity()
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: void PinnacleTouch::tuneEdgeSensitivity(uint8_t xAxisWideZMin, uint8_t yAxisWideZMin)

   This function was ported from Cirque's example code and doesn't seem to have corresponding
   documentation. I'm having trouble finding a memory map of the Pinnacle ASIC as this
   function directly alters values in the Pinnacle ASIC's memory. USE AT YOUR OWN RISK!

AnyMeas mode
*************

anyMeasModeConfig()
^^^^^^^^^^^^^^^^^^^^^^^
.. cpp:function:: void PinnacleTouch::anyMeasModeConfig(uint8_t gain, uint8_t frequency, uint32_t sampleLength, uint8_t muxControl, uint8_t appertureWidth, uint8_t controlPowerCount)

   Be sure to set the :cpp:func:`~PinnacleTouch::setDataMode()` attribute to
   :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS` before calling this function
   otherwise it will do nothing.

   :param int gain: Sets the sensitivity of the ADC matrix. Valid values are the constants
      defined in `AnyMeas mode Gain`_. Defaults to
      :cpp:var:`~PinnacleTouch::PINNACLE_GAIN_200`.
   :param int frequency: Sets the frequency of measurements made by the ADC matrix. Valid
      values are the constants defined in
      `AnyMeas mode Frequencies`_.
      Defaults :cpp:var:`~PinnacleTouch::PINNACLE_FREQ_0`.
   :param int sampleLength: Sets the maximum bit length of the measurements made by the ADC
      matrix. Valid values are ``128``, ``256``, or ``512``. Defaults to ``512``.
   :param int muxControl: The Pinnacle ASIC can employ different bipolar junctions
      and/or reference capacitors. Valid values are the constants defined in
      `AnyMeas mode Muxing`_. Additional combination of
      these constants is also allowed. Defaults to
      :cpp:var:`~PinnacleTouch::PINNACLE_MUX_PNP`.
   :param int appertureWidth: Sets the window of time (in nanoseconds) to allow for the ADC
      to take a measurement. Valid values are multiples of 125 in range [``250``, ``1875``].
      Erroneous values are clamped/truncated to this range.

      .. note:: The ``appertureWidth`` parameter has a inverse relationship/affect on the
            ``frequency`` parameter. The approximated frequencies described in this
            documentation are based on an aperture width of 500 nanoseconds, and they will
            shrink as the apperture width grows or grow as the aperture width shrinks.

   :param int controlPowerCount: Configure the Pinnacle to perform a number of measurements for
      each call to :cpp:func:`~PinnacleTouch::measureADC()`. Defaults to 1. Constants defined in
      `AnyMeas mode Control`_ can be used to specify if is sleep
      is allowed (:cpp:var:`~PinnacleTouch::PINNACLE_CRTL_PWR_IDLE` -- this
      is not default) or if repetive measurements is allowed (
      :cpp:var:`~PinnacleTouch::PINNACLE_CRTL_REPEAT`) if number of
      measurements is more than 1.

      .. warning:: There is no bounds checking on the number of measurements specified
            here. Specifying more than 63 will trigger sleep mode after performing
            measuements.

      .. tip:: Be aware that allowing the Pinnacle to enter sleep mode after taking
            measurements will slow consecutive calls to :cpp:func:`~PinnacleTouch::measureADC()` as the Pinnacle
            requires about 100 milliseconds to wake up.

measure_adc()
^^^^^^^^^^^^^^^^^^^^^^^

.. cpp:function:: int16_t PinnacleTouch::measureADC(unsigned int bitsToToggle, unsigned int togglePolarity)

   Be sure to set the :cpp:func:`~PinnacleTouch::setDataMode()` attribute to
   :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS` before calling this function
   otherwise it will do nothing.

   :param int bitsToToggle: This 4-byte integer specifies which bits the Pinnacle touch
      controller should toggle. A bit of ``1`` flags that bit for toggling, and a bit of
      ``0`` signifies that the bit should remain unaffected.
   :param int togglePolarity: This 4-byte integer specifies which polarity the specified
      bits (from ``bitsToToggle`` parameter) are toggled. A bit of ``1`` toggles that bit
      positve, and a bit of ``0`` toggles that bit negative.

   :Returns:
      A signed short integer. If :cpp:func:`~PinnacleTouch::setDataMode()` is not set
      to :cpp:var:`~PinnacleTouch::PINNACLE_ANYMEAS`, then this function returns
      ``NULL`` and does nothing.

   :4-byte Integer Format:
      .. csv-table:: byte 3 (MSByte)
            :stub-columns: 1
            :widths: 10, 5, 5, 5, 5, 5, 5, 5, 5

            "bit position",31,30,29,28,27,26,25,24
            "representation",N/A,N/A,Ref1,Ref0,Y11,Y10,Y9,Y8
      .. csv-table:: byte 2
            :stub-columns: 1
            :widths: 10, 5, 5, 5, 5, 5, 5, 5, 5

            "bit position",23,22,21,20,19,18,17,16
            "representation",Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0
      .. csv-table:: byte 1
            :stub-columns: 1
            :widths: 10, 5, 5, 5, 5, 5, 5, 5, 5

            "bit position",15,14,13,12,11,10,9,8
            "representation",X15,X14,X13,X12,X11,X10,X9,X8
      .. csv-table:: byte 0 (LSByte)
            :stub-columns: 1
            :widths: 10, 5, 5, 5, 5, 5, 5, 5, 5

            "bit position",7,6,5,4,3,2,1,0
            "representation",X7,X6,X5,X4,X3,X2,X1,X0

      See `AnyMeas mode example <examples.html#anymeas-mode>`_ to understand how to
      use these 4-byte integer polynomials.

      .. note:: Bits 29 and 28 represent the optional implementation of reference capacitors
            built into the Pinnacle ASIC. To use these capacitors, the
            corresponding constants
            (:cpp:var:`~PinnacleTouch::PINNACLE_MUX_REF0` and/or
            :cpp:var:`~PinnacleTouch::PINNACLE_MUX_REF1`) must be passed to
            :cpp:func:`~PinnacleTouch::anyMeasModeConfig()` in the ``mux_ctrl`` parameter, and their representative
            bits must be flagged in both ``bits_to_toggle`` & ``toggle_polarity`` parameters.

SPI & I2C Interfaces
********************

.. cpp:class:: PinnacleTouchSPI : public PinnacleTouch

   :param int slaveSelectPin: The "slave select" pin output to the Pinnacle ASIC.

   See the base class, :cpp:class:`PinnacleTouch` for other instantiating parameters.

.. cpp:class:: PinnacleTouchI2C : public PinnacleTouch

   :param int slaveAddress: The slave I2C address of the Pinnacle ASIC. Defaults to ``0x2A``.

   See the base class, :cpp:class:`PinnacleTouch` for other instantiating parameters.
