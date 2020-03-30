
PinnacleTouch API
==================

   .. cpp:namespace:: PinnacleTouch

.. |LessEq| unicode:: U+2264

Datatypes for data reports
--------------------------

Relative Data
**************

   .. cpp:type:: relativeReport

      This data structure used for returning data reports in relative mode (using
      :cpp:func:`reportRelative()` -- see also `Relative Mode in examples
      <examples.html#relative-mode>`_). The datatype, name, and range for each member are as follows:

            .. csv-table::
               :header: datatype,name,range

               uint8_t,buttons,"[0, 7] [1a]_"
               int8_t,x,"-128 |LessEq| X |LessEq| 127"
               int8_t,y,"-128 |LessEq| Y |LessEq| 127"
               int8_t,scroll,"-128 |LessEq| SCROLL |LessEq| 127 [2]_"

   .. [1a] The returned button data is a byte in which each bit represents a button.
      The bit to button order is as follows:

         0. [LSB] Button 1 (thought of as Left mouse button). If ``allTaps`` parameter is passed as
            ``true`` when calling :cpp:func:`relativeModeConfig()`, a single tap will be reflected here.
         1. Button 2 (thought of as Right mouse button). If ``allTaps`` and ``secondaryTap`` parameters
            are passed as ``true`` when calling :cpp:func:`relativeModeConfig()`, a single tap in the
            perspective top-left-most corner will be reflected here (secondary taps are constantly
            disabled if :cpp:func:`isHardConfigured()` returns ``true``). Note that the top-left-most
            corner can be perspectively moved if ``rotate90`` parameter is passed as ``true`` when
            calling :cpp:func:`relativeModeConfig()`.
         2. Button 3 (thought of as Middle mouse or scroll wheel button)

   .. [2] In Relative/Mouse mode the scroll wheel data is only reported if the
      ``intellimouse`` parameter is passed as ``true`` to :cpp:func:`relativeModeConfig()`.
      Otherwise this is an empty byte as the
      returned ``struct`` follows the buffer structure of a mouse HID report.

Absolute Data
**************

   .. cpp:type:: absoluteReport

      This data structure used for returning data reports in absolute mode (using
      :cpp:func:`reportAbsolute()`-- see also `Absolute Mode in examples
      <examples.html#absolute-mode>`_). The datatype, name, and range for each member are as follows:

            .. csv-table::
               :header: datatype,name,range

               uint8_t,buttons,"[0, 7] [1b]_"
               uint16_t,x,"128 |LessEq| X |LessEq| 1920"
               uint16_t,y,"64 |LessEq| Y |LessEq| 1472"
               uint8_t,z,"depends on sensitivity"
   .. [1b] The returned button data is a byte in which each bit represents a button.
      The bit to button order is as follows:

         0. [LSB] Button 1
         1. Button 2
         2. Button 3

Accepted Constants
------------------

Data Modes
***********

   Allowed symbols for configuring the Pinanacle ASIC's data reporting/measurements.

   .. cpp:var:: uint8_t PINNACLE_RELATIVE=0

      Alias symbol for specifying Relative mode (AKA Mouse mode).

   .. cpp:var:: uint8_t PINNACLE_ANYMEAS=1

      Alias symbol for specifying "AnyMeas" mode (raw ADC measurement)

   .. cpp:var:: uint8_t PINNACLE_ABSOLUTE=2

      Alias symbol for specifying Absolute mode (axis positions)

AnyMeas mode Gain
******************

   Allowed ADC gain configurations of AnyMeas mode. The percentages defined here are approximate
   values.

   .. cpp:var:: uint8_t PINNACLE_GAIN_100

      around 100% gain

   .. cpp:var:: uint8_t PINNACLE_GAIN_133

      around 133% gain

   .. cpp:var:: uint8_t PINNACLE_GAIN_166

      around 166% gain

   .. cpp:var:: uint8_t PINNACLE_GAIN_200

      around 200% gain

AnyMeas mode Frequencies
************************

   Allowed frequency configurations of AnyMeas mode. The frequencies defined here are
   approximated based on an aperture width of 500 nanoseconds. If the ``apertureWidth``
   parameter to :cpp:func:`anyMeasModeConfig()` specified is less than 500 nanoseconds, then the
   frequency will be larger than what is described here (& vice versa).

   .. cpp:var:: uint8_t PINNACLE_FREQ_0

      frequency around 500,000Hz

   .. cpp:var:: uint8_t PINNACLE_FREQ_1

      frequency around 444,444Hz

   .. cpp:var:: uint8_t PINNACLE_FREQ_2

      frequency around 400,000Hz

   .. cpp:var:: uint8_t PINNACLE_FREQ_3

      frequency around 363,636Hz

   .. cpp:var:: uint8_t PINNACLE_FREQ_4

      frequency around 333,333Hz

   .. cpp:var:: uint8_t PINNACLE_FREQ_5

      frequency around 307,692Hz

   .. cpp:var:: uint8_t PINNACLE_FREQ_6

      frequency around 267,000Hz

   .. cpp:var:: uint8_t PINNACLE_FREQ_7

      frequency around 235,000Hz


AnyMeas mode Muxing
*******************

   Allowed muxing gate polarity and reference capacitor configurations of AnyMeas mode.
   Combining these values (with ``+`` operator) is allowed.

   .. note:: The sign of the measurements taken in AnyMeas mode is inverted depending on which
      muxing gate is specified (when specifying an individual gate polarity).

   .. cpp:var:: uint8_t PINNACLE_MUX_REF1

      enables a builtin capacitor (~0.5pF). See note in :cpp:func:`measureADC()`

   .. cpp:var:: uint8_t PINNACLE_MUX_REF0

      enables a builtin capacitor (~0.25pF). See note in :cpp:func:`measureADC()`

   .. cpp:var:: uint8_t PINNACLE_MUX_PNP

      enable PNP sense line

   .. cpp:var:: uint8_t PINNACLE_MUX_NPN

      enable NPN sense line


AnyMeas mode Control
********************

   These constants control the number of measurements performed in :cpp:func:`measureADC()`.
   The number of measurements can range [0, 63].

   .. cpp:var:: uint8_t PINNACLE_CRTL_REPEAT

      required for more than 1 measurement

   .. cpp:var:: uint8_t PINNACLE_CRTL_PWR_IDLE

      triggers low power mode (sleep) after completing measurements


PinnacleTouch
-------------

Constructor & begin()
*************************

   .. cpp:class:: PinnacleTouch

      The abstract base class for driving the Pinnacle ASIC.

      :param uint8_t dataReadyPin: The input pin connected to the Pinnacle ASIC's "Data
         Ready" pin.

   .. cpp:function:: bool begin()

      :Returns:
         * ``true`` if the Pinnacle ASIC was setup and configured properly (with data
           feed enabled using Relative mode).
         * ``false`` if the Pinnacle ASIC was unresponsive for some reason (all further
           operations will be nullified by setting `DataMode`_ to ``0xFF``).

DataMode
*************************

Setter
   .. cpp:function:: void setDataMode(uint8_t mode)

      This function controls which mode the data report is configured for.

      :param uint8_t mode: Valid input values are :cpp:var:`PINNACLE_RELATIVE` for relative/mouse mode,
         :cpp:var:`PINNACLE_ABSOLUTE` for absolute positioning mode, or :cpp:var:`PINNACLE_ANYMEAS`
         (referred to as "AnyMeas" in specification sheet) mode for reading ADC values. Invalid input
         values have no affect.

Getter
   .. cpp:function:: uint8_t getDataMode()

      :Returns:
         - ``0`` (AKA :cpp:var:`PINNACLE_RELATIVE`) for Relative mode (AKA mouse mode)
         - ``1`` (AKA :cpp:var:`PINNACLE_ANYMEAS`) for AnyMeas mode (raw ADC measurements)
         - ``2`` (AKA :cpp:var:`PINNACLE_ABSOLUTE`) for Absolute mode (X & Y axis positions)
         - ``255`` if :cpp:func:`begin()` returns ``false`` (failed to initialize the trackpad)

      .. important:: When switching from :cpp:var:`PINNACLE_ANYMEAS` to :cpp:var:`PINNACLE_RELATIVE`
         or :cpp:var:`PINNACLE_ABSOLUTE` all configurations are reset, and must be re-configured by
         using  :cpp:func:`absoluteModeConfig()` or :cpp:func:`relativeModeConfig()`.

Relative or Absolute mode
*************************

feedEnabled
^^^^^^^^^^^^^^^^^^^^^^^

Setter
   .. cpp:function:: void feedEnabled(bool isEnabled)

      This attribute controls if the touch/button event data is reported or not. This function
      only applies to :cpp:var:`PINNACLE_RELATIVE` or :cpp:var:`PINNACLE_ABSOLUTE` mode, otherwise if
      `DataMode`_ is set to :cpp:var:`PINNACLE_ANYMEAS`, then this function will do nothing.

      :param bool isEnabled: ``true`` enables data reporting; ``false`` disables data reporting.

Getter
   .. cpp:function:: bool isFeedEnabled()

      :Returns:
         The setting configured by :cpp:func:`feedEnabled()` or ``false`` if `DataMode`_ is set
         to :cpp:var:`PINNACLE_ANYMEAS`.

isHardConfigured()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: bool isHardConfigured()

      This function can be used to inform applications about the factory customized hardware
      configuration. See note about product labeling in `Model Labeling Scheme <index.html#cc>`_.

      :Returns:
         ``true`` if a 470K ohm resistor is populated at the junction labeled "R4"; ``false`` if
         no resistor is populated at the "R4" junction.

relativeModeConfig()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: void relativeModeConfig(bool rotate90, bool allTaps, bool secondaryTap, bool glideExtend, bool intellimouse)

      Configure settings specific to Relative mode (AKA Mouse mode) data reporting. This function
      only applies to :cpp:var:`PINNACLE_RELATIVE` mode, otherwise if `DataMode`_ is set to
      :cpp:var:`PINNACLE_ANYMEAS` or :cpp:var:`PINNACLE_ABSOLUTE`, then this function does nothing.

      :param bool rotate90: Specifies if the axis data is altered for 90 degree rotation before
         reporting it (essentially swaps the axis data). Default is ``false``.
      :param bool allTaps: Specifies if all taps should be reported (``true``) or not
         (``false``). Default is ``true``. This affects ``secondaryTap`` option as well.
      :param bool secondaryTap: Specifies if tapping in the top-left corner (depending on
         orientation) triggers the secondary button data. Defaults to ``true``. This feature is
         always disabled if :cpp:func:`isHardConfigured()` is ``true``.
      :param bool glideExtend: A patended feature that allows the user to glide their finger off
         the edge of the sensor and continue gesture with the touch event. Default is ``true``.
         This feature is always disabled if :cpp:func:`isHardConfigured()` is ``true``.
      :param bool intellimouse: Specifies if the data reported includes a byte about scroll data.
         Default is ``false``. This feature is always disabled if :cpp:func:`isHardConfigured()`
         is ``true``.

absoluteModeConfig()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: void absoluteModeConfig(uint8_t zIdleCount, bool invertX, bool invertY)

      Configure settings specific to Absolute mode (reports axis positions). This function only
      applies to :cpp:var:`PINNACLE_ABSOLUTE` mode, otherwise if `DataMode`_ is set to
      :cpp:var:`PINNACLE_ANYMEAS` or :cpp:var:`PINNACLE_RELATIVE`, then this function does nothing.

      :param int zIdleCount: Specifies the number of empty packets (x-axis, y-axis, and z-axis
         are ``0``) reported (every 10 milliseconds) when there is no touch detected. Defaults
         to 30. This number is clamped to range [0, 255].
      :param bool invertX: Specifies if the x-axis data is to be inverted before reporting it.
         Default is ``false``.
      :param bool invertY: Specifies if the y-axis data is to be inverted before reporting it.
         Default is ``false``.

available()
^^^^^^^^^^^^^^^^^^^^^^^
   .. cpp:function:: bool available()

      Use this function to detirmine if there is new data to report. Internally, this function
      checks if the interrupt signal on the "data ready" pin (labeled "DR" in the `pinout
      <index.html#pinout>`_ section) is active. Data (new or antiquated) can be retreived using
      :cpp:func:`reportRelative()` or :cpp:func:`reportAbsolute()` depending on what `DataMode`_
      is set to.

      :Returns:
         ``true`` if there is new data to report; ``false`` if there is no new data to report.

reportAbsolute()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: AbsoluteReport reportAbsolute(absoluteReport* report)

      This function will fetch touch (and button) event data from the Pinnacle ASIC (including
      empty packets on ending of a touch/button event). This function only applies to
      :cpp:var:`PINNACLE_ABSOLUTE` mode, otherwise if `DataMode`_ is set to
      :cpp:var:`PINNACLE_ANYMEAS`, then this function does nothing.

      :param absoluteReport* report: A reference pointer (declared variable of datatype
         :cpp:type:`absoluteReport`) for storing the data that describes the touch (and button)
         event.

reportRelative()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: AbsoluteReport reportRelative(relativeReport* report)

      This function will fetch touch (and button) event data from the Pinnacle ASIC. This
      function only applies to :cpp:var:`PINNACLE_RELATIVE` mode, otherwise if `DataMode`_ is set
      to :cpp:var:`PINNACLE_ANYMEAS`, then this function does nothing.

      :param relativeReport* report: A reference pointer (declared variable of datatype
         :cpp:type:`relativeReport`) for storing the data that describes the touch (and button)
         event.

clearFlags()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: void clearFlags()

      Use this function to clear the interrupt signal (digital input; active when HIGH) on the
      "data ready" pin (marked "DR" in the `pinout <index.html#pinout>`_ section). This function
      is mainly used internally when applicable, but it is left exposed if the application wants
      to neglect a data report when desirable.

AllowSleep
^^^^^^^^^^^^^^^^^^^^^^^

Setter
   .. cpp:function:: void allowSleep(bool isEnabled)

      This will specify if the Pinnacle ASIC is allowed to sleep after about 5 seconds
      of idle activity (no input event).

      :param bool isEnabled: ``true`` if you want the Pinnacle ASIC to enter sleep (low power)
         mode after about 5 seconds of inactivity (does not apply to AnyMeas mode). ``false`` if
         you don't want the Pinnacle ASIC to enter sleep mode.

      .. note:: While the touch controller is in sleep mode, if a touch event or button press is
         detected, the Pinnacle ASIC will take about 300 milliseconds to wake up (does not include
         handling the touch event or button press data). Remember that releasing a held button is
         also considered an input event.

Getter
   .. cpp:function:: bool isAllowSleep()

      :Returns: The setting configured by :cpp:func:`allowSleep()`

shutdown
^^^^^^^^^^^^^^^^^^^^^^^

Setter
   .. cpp:function:: void shutdown(bool isOff)

      This function controls power state of the Pinnacle ASIC that drives the touchpad.

      :param bool isOff: ``true`` means power down (AKA standby mode), and ``false`` means
         power up (Active, Idle, or Sleep mode).

      .. note:: The ASIC will take about 300 milliseconds to complete the transition
         from powered down mode to active mode. No touch events or button presses will be
         monitored while powered down.

Getter
   .. cpp:function:: bool isShutdown()

      :Returns: The setting configured by :cpp:func:`shutdown()`

SampleRate
^^^^^^^^^^^^^^^^^^^^^^^

Setter
   .. cpp:function:: void setSampleRate(uint16_t value)

      This function controls how many samples (of data) per second are taken. This function
      only applies to :cpp:var:`PINNACLE_RELATIVE` or :cpp:var:`PINNACLE_ABSOLUTE` mode, otherwise
      if `DataMode`_ is set to :cpp:var:`PINNACLE_ANYMEAS`, then this function will do nothing.

      :param uint16_t value: Valid input values are ``100``, ``80``, ``60``, ``40``, ``20``,
         ``10``. Any other input values automatically set the sample rate to 100 sps (samples
         per second). Optionally, ``200`` and ``300`` sps can be specified, but using these
         optional values automatically disables palm (referred to as "NERD" in the specification
         sheet) and noise compensations. These higher values are meant for using a stylus with a
         2mm diameter tip, while the values less than 200 are meant for a finger or stylus with
         a 5.25mm diameter tip.

Getter
   .. cpp:function:: bool getSampleRate()

      :Returns:
         The setting configured by :cpp:func:`setSampleRate()` or ``0`` if `DataMode`_ is
         set to :cpp:var:`PINNACLE_ANYMEAS`.

detectFingerStylus()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: void detectFingerStylus(bool enableFinger, bool enableStylus, uint16_t sampleRate)

      This function will configure the Pinnacle ASIC to detect either finger, stylus, or both.

      :param bool enableFinger: ``true`` enables the Pinnacle ASIC's measurements to
         detect if the touch event was caused by a finger or 5.25mm stylus. ``false`` disables
         this feature. Default is ``true``.
      :param bool enableStylus: ``true`` enables the Pinnacle ASIC's measurements to
         detect if the touch event was caused by a 2mm stylus. ``false`` disables this
         feature. Default is ``true``.
      :param int sampleRate: See the `SampleRate`_ attribute as this parameter directly calls that
         attribute's setter function.

      .. tip:: Consider adjusting the ADC matrix's gain to enhance performance/results using
         :cpp:func:`setAdcGain()`

calibrate()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: void calibrate(bool run, bool tap, bool trackError, bool nerd, bool background)

      Set calibration parameters when the Pinnacle ASIC calibrates itself. This function only applies
      to :cpp:var:`PINNACLE_RELATIVE` or :cpp:var:`PINNACLE_ABSOLUTE` mode, otherwise if `DataMode`_
      is set to :cpp:var:`PINNACLE_ANYMEAS`, then this function will do nothing.

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

Setter
   .. cpp:function:: void setCalibrationMatrix(int16_t* matrix)

      Manually sets the compensation matrix (array) of the 46 signed short integer values
      stored in the Pinnacle ASIC's memory that is used for taking measurements. This matrix
      may not applicable in AnyMeas mode (specification sheet is lacking adequate
      information).

      :param int16_t* matrix: The array of 46 signed short integers (AKA int16_t) that will
         be used for compensation calculations when measuring of input events. See note below
         from the Pinnacle ASIC's application note about deciding what values to use.

Getter
   .. cpp:function:: void getCalibrationMatrix(int16_t* matrix)

      Use this function to compare a prior compensation matrix with a new matrix that was
      either loaded manually via :cpp:func:`setCalibrationMatrix()` or created internally by
      calling :cpp:func:`calibrate()` with the ``run`` parameter as ``true``.

      :param int16_t* matrix: A reference pointer (declared array of 46 signed short integers)
         for storing the compensation matrix configured by :cpp:func:`setCalibrationMatrix()`
         or created internally by :cpp:func:`calibrate()` (or after a "power-on-reset" condition).

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

setAdcGain()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: void setAdcGain(uint8_t sensitivity)

      Sets the ADC (Analog to Digital Converter) attenuation (gain ratio) to enhance
      performance based on the overlay type. This does not apply to AnyMeas mode. However, the
      input value specified can be written while `DataMode`_ is set to
      :cpp:var:`PINNACLE_ANYMEAS`, but there is no garauntee that it will "stick" as it may be
      overidden by the Pinnacle ASIC (specification sheet does not imply either way).

      :param uint8_t sensitivity: This byte specifies how sensitive the ADC component is. It
         must be in range [0, 3]. Where ``0`` means most sensitive, and ``3`` means least
         sensitive. A value outside this range will default to ``0``.

      .. tip:: The official example code from Cirque for a curved overlay uses a value of ``1``.

tuneEdgeSensitivity()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: void tuneEdgeSensitivity(uint8_t xAxisWideZMin, uint8_t yAxisWideZMin)

      According to the comments in the official example code from Cirque, "Changes thresholds to
      improve detection of fingers." This function was ported from Cirque's example code and
      doesn't have corresponding documentation. Thus, the defaults for this function's parameters
      use the same values found in the official example. I'm unaware of any documented memory map
      for the Pinnacle ASIC as this function directly alters values in the Pinnacle ASIC's memory.
      USE THIS AT YOUR OWN RISK!

AnyMeas mode
*************

anyMeasModeConfig()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: void anyMeasModeConfig(uint8_t gain, uint8_t frequency, uint32_t sampleLength, uint8_t muxControl, uint8_t appertureWidth, uint8_t controlPowerCount)

      This function configures the Pinnacle ASIC for taking raw ADC measurements. Be sure to set
      the `DataMode`_ attribute to :cpp:var:`PINNACLE_ANYMEAS` before calling this function
      otherwise it will do nothing.

      :param int gain: Sets the sensitivity of the ADC matrix. Valid values are the constants
         defined in `AnyMeas mode Gain`_. Defaults to :cpp:var:`PINNACLE_GAIN_200`.
      :param int frequency: Sets the frequency of measurements made by the ADC matrix. Valid
         values are the constants defined in
         `AnyMeas mode Frequencies`_. Defaults :cpp:var:`PINNACLE_FREQ_0`.
      :param int sampleLength: Sets the maximum bit length of the measurements made by the ADC
         matrix. Valid values are ``128``, ``256``, or ``512``. Defaults to ``512``.
      :param int muxControl: The Pinnacle ASIC can employ different bipolar junctions
         and/or reference capacitors. Valid values are the constants defined in
         `AnyMeas mode Muxing`_. Additional combination of
         these constants is also allowed. Defaults to :cpp:var:`PINNACLE_MUX_PNP`.
      :param int appertureWidth: Sets the window of time (in nanoseconds) to allow for the ADC
         to take a measurement. Valid values are multiples of 125 in range [``250``, ``1875``].
         Erroneous values are clamped/truncated to this range.

         .. note:: The ``appertureWidth`` parameter has a inverse relationship/affect on the
               ``frequency`` parameter. The approximated frequencies described in this
               documentation are based on an aperture width of 500 nanoseconds, and they will
               shrink as the apperture width grows or grow as the aperture width shrinks.

      :param int controlPowerCount: Configure the Pinnacle to perform a number of measurements for
         each call to :cpp:func:`measureADC()`. Defaults to 1. Constants defined in
         `AnyMeas mode Control`_ can be added (with ``+``) to specify if is sleep
         is allowed (:cpp:var:`PINNACLE_CRTL_PWR_IDLE` -- this
         is not default) or if repetative measurements is allowed
         (:cpp:var:`PINNACLE_CRTL_REPEAT`) when number of measurements is more than 1.

         .. warning:: There is no bounds checking on the number of measurements specified
               here. Specifying more than 63 will trigger sleep mode after performing
               measuements.

         .. tip:: Be aware that allowing the Pinnacle to enter sleep mode after taking
               measurements will cause a latency in consecutive calls to
               :cpp:func:`measureADC()` as the Pinnacle requires about 300 milliseconds to wake
               up.

measure_adc()
^^^^^^^^^^^^^^^^^^^^^^^

   .. cpp:function:: int16_t measureADC(unsigned int bitsToToggle, unsigned int togglePolarity)

      This function instigates and returns the measurement (a signed short integer) from the
      Pinnacle ASIC's ADC (Analog to Digital Converter) matrix (only applies to AnyMeas mode).
      Be sure to set the `DataMode`_ attribute to :cpp:var:`PINNACLE_ANYMEAS` before calling
      this function otherwise it will do nothing and return ``0``.

      :param int bitsToToggle: This 4-byte integer specifies which bits the Pinnacle touch
         controller should toggle. A bit of ``1`` flags that bit for toggling, and a bit of
         ``0`` signifies that the bit should remain unaffected.
      :param int togglePolarity: This 4-byte integer specifies which polarity the specified
         bits (from ``bitsToToggle`` parameter) are toggled. A bit of ``1`` toggles that bit
         positve, and a bit of ``0`` toggles that bit negative.

      :Returns:
         A signed short integer. If `DataMode`_ is not set to :cpp:var:`PINNACLE_ANYMEAS`,
         then this function returns ``0`` and does nothing.

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
               built into the Pinnacle ASIC. To use these capacitors, the corresponding constants
               (:cpp:var:`PINNACLE_MUX_REF0` and/or :cpp:var:`PINNACLE_MUX_REF1`) must be passed to
               :cpp:func:`anyMeasModeConfig()` in the ``muxControl`` parameter, and their representative
               bits must be flagged in both the ``bitsToToggle`` & ``togglePolarity`` parameters.

SPI & I2C Interfaces
********************

   .. cpp:class:: PinnacleTouchSPI : public PinnacleTouch

      Parent class for interfacing with the Pinnacle ASIC via the SPI protocol.

      :param int slaveSelectPin: The "slave select" pin output to the Pinnacle ASIC.

      See the base class, :cpp:class:`PinnacleTouch` for other instantiating parameters.

   .. cpp:class:: PinnacleTouchI2C : public PinnacleTouch

      Parent class for interfacing with the Pinnacle ASIC via the I2C protocol.

      :param int slaveAddress: The slave I2C address of the Pinnacle ASIC. Defaults to ``0x2A``.

      See the base class, :cpp:class:`PinnacleTouch` for other instantiating parameters.
