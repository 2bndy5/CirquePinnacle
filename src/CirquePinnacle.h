/*
 * Copyright (c) 2023 Brendan Doherty (2bndy5)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _CIRQUEPINNACLE_H_
#define _CIRQUEPINNACLE_H_
#include <stdint.h>
#include "CirquePinnacle_common.h"

/** Defined constants for Pinnacle registers */
#define PINNACLE_FIRMWARE_ID    0x00
#define PINNACLE_STATUS         0x02
#define PINNACLE_SYS_CONFIG     0x03
#define PINNACLE_FEED_CONFIG_1  0x04
#define PINNACLE_FEED_CONFIG_2  0x05
#define PINNACLE_FEED_CONFIG_3  0x06
#define PINNACLE_CAL_CONFIG     0x07
#define PINNACLE_SAMPLE_RATE    0x09
#define PINNACLE_Z_IDLE         0x0A
#define PINNACLE_Z_SCALER       0x0B
#define PINNACLE_SLEEP_INTERVAL 0x0C // time of sleep until checking for finger
#define PINNACLE_SLEEP_TIMER    0x0D // time after idle mode until sleep starts
#define PINNACLE_PACKET_BYTE_0  0x12
#define PINNACLE_PACKET_BYTE_1  0x13
#define PINNACLE_ERA_VALUE      0x1B
#define PINNACLE_ERA_ADDR       0x1C
#define PINNACLE_ERA_CONTROL    0x1E
#define PINNACLE_HCO_ID         0x1F

// *************** defined Constants for bitwise configuration *****************
/**
 * Allowed symbols for configuring the Pinnacle ASIC's data
 * reporting/measurements.
 *
 * .. seealso:: `PinnacleTouch::getDataMode()`, `PinnacleTouch::setDataMode()`
 *
 * @ingroup data-mode
 */
enum PinnacleDataMode : uint8_t
{
    /** Alias symbol for specifying Relative mode (AKA Mouse mode). */
    PINNACLE_RELATIVE = 0x00,
#if PINNACLE_ANYMEAS_SUPPORT
    /**
     * Alias symbol for specifying "AnyMeas" mode (raw ADC measurement).
     *
     * .. anymeas-limits::
     *
     *     Anymeas mode requires the use of a Hardware Data Ready pin.
     *     Do not use `PINNACLE_SW_DR` as the value for the ``dataReadyPin`` parameter to
     *     `~PinnacleTouchSPI::PinnacleTouchSPI()` or `~PinnacleTouchI2C::PinnacleTouchI2C()`
     *     constructors.
     */
    PINNACLE_ANYMEAS = 0x01,
#endif
    /** Alias symbol for specifying Absolute mode (axis positions) */
    PINNACLE_ABSOLUTE = 0x02,
    /** Alias for value used to prevent operations when `~PinnacleTouch::begin()` fails. */
    PINNACLE_ERROR = 0xFF,
};

#if PINNACLE_ANYMEAS_SUPPORT

/**
 * Allowed ADC gain configurations of AnyMeas mode.
 *
 * The percentages defined here are approximate values.
 *
 * .. seealso:: `PinnacleTouch::anymeasModeConfig()`
 *
 * @ingroup anymeas-const
 */
enum PinnacleAnyMeasGain : uint8_t
{
    /** around 100% gain */
    PINNACLE_GAIN_100 = 0xC0,
    /** around 133% gain */
    PINNACLE_GAIN_133 = 0x80,
    /** around 166% gain */
    PINNACLE_GAIN_166 = 0x40,
    /** around 200% gain */
    PINNACLE_GAIN_200 = 0x00,
};

/**
 * Allowed frequency configurations of AnyMeas mode.
 *
 * The frequencies defined here are approximated based on an aperture
 * width of 500 nanoseconds. If the ``apertureWidth`` parameter to
 * `PinnacleTouch::anymeasModeConfig()` specified is less than 500 nanoseconds,
 * then the frequency will be larger than what is described here (and vice
 * versa).
 *
 * .. seealso:: `PinnacleTouch::anymeasModeConfig()`
 *
 * @ingroup anymeas-const
 */
enum PinnacleAnyMeasFreq : uint8_t
{
    /** frequency around 500,000 Hz */
    PINNACLE_FREQ_0 = 0x02,
    /** frequency around 444,444 Hz */
    PINNACLE_FREQ_1 = 0x03,
    /** frequency around 400,000 Hz */
    PINNACLE_FREQ_2 = 0x04,
    /** frequency around 363,636 Hz */
    PINNACLE_FREQ_3 = 0x05,
    /** frequency around 333,333 Hz */
    PINNACLE_FREQ_4 = 0x06,
    /** frequency around 307,692 Hz */
    PINNACLE_FREQ_5 = 0x07,
    /** frequency around 267,000 Hz */
    PINNACLE_FREQ_6 = 0x09,
    /** frequency around 235,000 Hz */
    PINNACLE_FREQ_7 = 0x0B,
};

/**
 * Allowed muxing gate polarity and reference capacitor configurations
 * of AnyMeas mode.
 *
 * Combining these values (with ``+`` operator) is allowed.
 *
 * .. note:: The sign of the measurements taken in AnyMeas mode is inverted
 *     depending on which muxing gate is specified (when specifying an individual
 *     gate polarity).
 * .. seealso:: `PinnacleTouch::anymeasModeConfig()`, `PinnacleTouch::measureAdc()`
 *
 * @ingroup anymeas-const
 */
enum PinnacleAnyMeasMuxing : uint8_t
{
    /**
     * enables a builtin capacitor (~0.5pF).
     *
     * .. seealso:: `PinnacleTouch::measureAdc()`
     */
    PINNACLE_MUX_REF1 = 0x10,
    /**
     * enables a builtin capacitor (~0.25pF).
     *
     * .. seealso:: `PinnacleTouch::measureAdc()`
     */
    PINNACLE_MUX_REF0 = 0x08,
    /** enable PNP sense line */
    PINNACLE_MUX_PNP = 0x04,
    /** enable NPN sense line */
    PINNACLE_MUX_NPN = 0x01,
};

/**
 * These constants control the number of measurements performed in
 * `PinnacleTouch::anymeasModeConfig()`.
 *
 * .. important:: The number of measurements can range [0, 63].
 * .. seealso:: `PinnacleTouch::anymeasModeConfig()`
 *
 * @ingroup anymeas-const
 */
enum PinnacleAnyMeasCtrl : uint8_t
{
    /** only required for more than 1 measurement */
    PINNACLE_CRTL_REPEAT = 0x80,
    /** triggers low power mode (sleep) after completing measurements */
    PINNACLE_CRTL_PWR_IDLE = 0x40,
};

#endif // PINNACLE_ANYMEAS_SUPPORT == false

/**
 * This data structure is used for returning data reports in relative mode using
 * :cpp:expr:`PinnacleTouch::read(RelativeReport*)`.
 *
 * @ingroup relative-mode
 */
struct RelativeReport
{
    /**
     * This will always be in range [0, 7].
     *
     * The returned button data is a byte in which each bit
     * represents if a button is pressed. The bit to button order is as
     * follows:
     *
     * .. list-table::
     *     :header-rows: 1
     *     :widths: 2, 5, 10
     *
     *     * - bit position
     *       - button number
     *       - description
     *     * - 0 [LSB]
     *       - Button 1 (thought of as Left mouse button)
     *       - If ``allTaps`` parameter is passed as ``true`` when calling
     *         :cpp:func:`~PinnacleTouch::relativeModeConfig()`, a single tap will be
     *         reflected here.
     *     * - 1
     *       - Button 2 (thought of as Right mouse button)
     *       - If ``allTaps`` and ``secondaryTap`` parameters are passed as ``true``
     *         when calling :cpp:func:`~PinnacleTouch::relativeModeConfig()`, a single tap in the
     *         perspective top-left-most corner will be reflected here (secondary taps
     *         are constantly disabled if :cpp:func:`~PinnacleTouch::isHardConfigured()` returns
     *         ``true``). Note that the top-left-most corner can be perspectively moved if
     *         ``rotate90`` parameter is passed as ``true`` when calling
     *         :cpp:func:`~PinnacleTouch::relativeModeConfig()`.
     *     * - 2
     *       - Button 3 (thought of as Middle mouse or scroll wheel button)
     *       -
     */
    uint8_t buttons = 0;
    /** This will always be in range ``-128 <= x <= 127`` */
    int8_t x = 0;
    /** This will always be in range ``-128 <= y <= 127`` */
    int8_t y = 0;
    /**
     * This will always be in range ``-128 <= scroll <= 127``
     *
     * .. note:: In Relative/Mouse mode the scroll wheel data is only reported
     *     if the ``intellimouse`` parameter is passed as ``true`` to
     *     `PinnacleTouch::relativeModeConfig()`. Otherwise this is an empty byte as
     *     the `RelativeReport` follows the buffer structure of a mouse HID report.
     */
    int8_t scroll = 0;
};

/**
 * This data structure is used for returning data reports in absolute mode using
 * :cpp:expr:`PinnacleTouch::read(AbsoluteReport*)`.
 *
 * @ingroup absolute-mode
 */
struct AbsoluteReport
{
    /**
     * This will always be in range [0, 7].
     *
     * The returned button data is a byte in which each bit
     * represents a button. The bit to button order is as follows:
     *
     * .. csv-table::
     *     :header: "bit position", "button number"
     *
     *     "0 (LSB)", "Button 1"
     *     1, "Button 2"
     *     2, "Button 3"
     */
    uint8_t buttons = 0;
    /**
     * This will always be in range ``0 <= x <= 2047``
     *
     * The datasheet recommends this value should be
     * clamped to range ``128 <= x <= 1920`` for reliability.
     */
    uint16_t x = 0;
    /**
     * This will always be in range ``0 <= y <= 1535``
     *
     * The datasheet recommends this value should be
     * clamped to range ``64 <= y <= 1472`` for reliability.
     */
    uint16_t y = 0;
    /**
     * This will always be in range ``0 <= z <= 31``. The maximum
     * value will depend on sensitivity.
     */
    uint8_t z = 0;
};

/**
 * The abstract base class for driving the Pinnacle ASIC.
 *
 * @ingroup pinnacle-touch-api
 */
class PinnacleTouch
{
public:
    /**
     * Create an instance to use as an interface with the Pinnacle ASIC
     * touch controller.
     *
     * @param dataReadyPin The input pin connected to the Pinnacle ASIC's "Data Ready" pin.
     *     To use the Pinnacle's Software Data Ready flag instead, set this to `PINNACLE_SW_DR`.
     *     BEWARE, the Hardware Data Ready pin is required for anymeas mode. In general, it
     *     recommended to use the Hardware Data Ready pin for speed and efficient bus activity.
     */
    PinnacleTouch(pinnacle_gpio_t dataReadyPin);
    /**
     * This function controls if the touch/button event data is reported or
     * not. It only applies to `PINNACLE_RELATIVE` or `PINNACLE_ABSOLUTE`
     * mode, otherwise if `setDataMode()` is given `PINNACLE_ANYMEAS`, then this
     * function will do nothing.
     *
     * @param isEnabled Enables (``true``) or disables (``false``) data reporting.
     */
    void feedEnabled(bool isEnabled);
    /**
     * This function describes if the touch/button event data is to be
     * reported or not.
     *
     * @return
     *     The setting configured by `feedEnabled()` or ``false`` if `setDataMode()`
     *     is given `PINNACLE_ANYMEAS`.
     */
    bool isFeedEnabled();
    /**
     * This function controls the mode for which kind of data to report.
     *
     * @param mode Valid input values are
     *
     *     .. csv-table::
     *         :header: "``mode`` (enum value)", description
     *
     *         ":expr:`PINNACLE_RELATIVE` (``0``)", "for relative/mouse mode"
     *         ":expr:`PINNACLE_ANYMEAS` (``1``)", "for reading raw ADC values"
     *         ":expr:`PINNACLE_ABSOLUTE` (``2``)", "for absolute positioning mode"
     *
     *     Invalid input values have no affect.
     */
    void setDataMode(PinnacleDataMode mode);
    /**
     * This function describes which mode for which kind of data to report.
     *
     * .. important::
     *     When switching from `PINNACLE_ANYMEAS` to `PINNACLE_RELATIVE`
     *     or `PINNACLE_ABSOLUTE` all configurations are reset, and must be
     *     re-configured by using  `absoluteModeConfig()` or `relativeModeConfig()`.
     *
     * @return
     *     - :expr:`PINNACLE_RELATIVE` (``0``) for relative mode (AKA mouse mode)
     *     - :expr:`PINNACLE_ANYMEAS` (``1``) for anymeas mode (raw ADC measurements)
     *     - :expr:`PINNACLE_ABSOLUTE` (``2``) for absolute mode (X & Y axis
     *       positions)
     *     - :expr:`PINNACLE_ERROR` (``0xFF``) if `begin()` returns ``false`` (failed to
     *       initialize the trackpad)
     */
    PinnacleDataMode getDataMode();
    /**
     * This function can be used to inform applications about the factory
     * customized hardware configuration.
     *
     * .. seealso:: Read the section about product labeling in
     *     :ref:`Model Labeling Scheme <HCO>` (specifically the ``cc`` point).
     *
     * @returns ``true`` if a 470K ohm resistor is populated at the junction
     *     labeled "R4"; ``false`` if no resistor is populated at the "R4" junction.
     *     This function will also return ``false`` if `begin()` failed to initialize
     *     the trackpad.
     *
     *     .. error::
     *         :no-title:
     *
     *         This seems to return ``true`` when the trackpad is configured for I2C interface.
     */
    bool isHardConfigured();
    /**
     * Use this function to determine if there is new data to report.
     * Internally, this function checks if the interrupt signal on the "data
     * ready" pin (labeled "DR" in the `pinout <index.html#pinout>`_ section)
     * is active. Data, new or antiquated, can be retrieved using
     * `read()` depending on what `setDataMode()` is given.
     *
     * .. note::
     *     If `~PinnacleTouch::PinnacleTouch()` is given `PINNACLE_SW_DR` as the
     *     ``dataReadyPin`` parameter, then this function will attempt to read data from
     *     the Pinnacle's Status register. However, this tactic is only allowed for
     *     `PINNACLE_RELATIVE` or `PINNACLE_ABSOLUTE` modes.
     *
     * @returns ``true`` if there is new data to report; ``false`` if there is no
     *     new data to report.
     */
    bool available();
    /**
     * Configure settings specific to Absolute mode (reports axis positions). This function only
     * applies to `PINNACLE_ABSOLUTE` mode, otherwise if `setDataMode()` is given
     * `PINNACLE_ANYMEAS` or `PINNACLE_RELATIVE`, then this function does nothing.
     *
     * @param zIdleCount Specifies the number of empty packets (x-axis, y-axis, and z-axis
     *     are ``0``) reported (every 10 milliseconds) when there is no touch detected. Defaults
     *     to 30. This number is clamped to range [0, 255].
     * @param invertX Specifies if the x-axis data is to be inverted before reporting it.
     *     Default is ``false``.
     * @param invertY Specifies if the y-axis data is to be inverted before reporting it.
     *     Default is ``false``.
     */
    void absoluteModeConfig(uint8_t zIdleCount = 30,
                            bool invertX = false,
                            bool invertY = false);
    /**
     * Configure settings specific to Relative mode (AKA Mouse mode) data reporting. This function
     * only applies to `PINNACLE_RELATIVE` mode, otherwise if `setDataMode()` is given
     * `PINNACLE_ANYMEAS` or `PINNACLE_ABSOLUTE`, then this function does nothing.
     *
     * @param taps Specifies if all taps should be reported (``true``) or not
     *     (``false``). Default is ``true``. This affects `secondaryTap` option as well.
     * @param rotate90 Specifies if the axis data is altered for 90 degree rotation before
     *     reporting it (essentially swaps the axis data). Default is ``false``.
     * @param secondaryTap Specifies if tapping in the top-left corner (depending on
     *     orientation) triggers the secondary button data. Defaults to ``true``.
     * @param intellimouse Specifies if the data reported includes a byte about scroll data.
     *     Default is ``false``. This feature is always disabled if `isHardConfigured()`
     *     is ``true``.
     * @param glideExtend A patented feature that allows the user to glide their finger off
     *     the edge of the sensor and continue gesture with the touch event. Default is ``false``.
     *     This feature is always disabled if `isHardConfigured()` is ``true``.
     */
    void relativeModeConfig(bool taps = true,
                            bool rotate90 = false,
                            bool secondaryTap = true,
                            bool intellimouse = false,
                            bool glideExtend = false);
    /**
     * This function will fetch touch (and button) event data from the
     * Pinnacle ASIC. This function only applies to `PINNACLE_RELATIVE` mode,
     * otherwise if `setDataMode()` is given `PINNACLE_ANYMEAS` or
     * `PINNACLE_ABSOLUTE`, then this function does nothing.
     *
     * @param[out] report A reference pointer (declared variable of datatype
     *     `RelativeReport`) for storing the data that describes the touch (and
     *     button) event.
     * @param readButtons A flag that can be used to skip reading the button data from
     *     the Pinnacle. The default (``true``) will read the button data and store it
     *     in the `report` object's `~RelativeReport::buttons` attribute. This
     *     parameter is useful to speed up read operations when not using the Pinnacle's
     *     button input pins.
     *
     *     .. warning::
     *         If relative mode's tap detection is enabled, then setting this parameter
     *         to ``false`` can yield deceptively inaccurate data when reporting tap
     *         gestures.
     *
     * @id RelativeReport
     */
    void read(RelativeReport* report, bool readButtons = true);
    /**
     * This function will fetch touch (and button) event data from the
     * Pinnacle ASIC (including empty packets on ending of a touch/button
     * event). This function only applies to `PINNACLE_ABSOLUTE` mode, otherwise
     * if `setDataMode()` is given `PINNACLE_ANYMEAS` or `PINNACLE_RELATIVE`,
     * then this function does nothing.
     *
     * @param[out] report A reference pointer (declared variable of datatype
     *     `AbsoluteReport`) for storing the data that describes the touch (and
     *     button) event.
     * @param readButtons A flag that can be used to skip reading the button data from
     *     the Pinnacle. The default (``true``) will read the button data and store it
     *     in the `report` object's `~AbsoluteReport::buttons` attribute. This
     *     parameter is useful to speed up read operations when not using the Pinnacle's
     *     button input pins.
     *
     * @id AbsoluteReport
     */
    void read(AbsoluteReport* report, bool readButtons = true);
    /**
     * Use this function to clear the interrupt signal (digital input; active
     * when HIGH) on the "data ready" pin (marked "DR" in the
     * `pinout <index.html#pinout>`_ section). This function is mainly used
     * internally when applicable, but it is left exposed if the application
     * wants to neglect a data report when desirable.
     */
    void clearStatusFlags();
    /**
     * This will specify if the Pinnacle ASIC is allowed to sleep after about
     * 5 seconds of idle activity (no input event).
     *
     * .. note::
     *     While the touch controller is in sleep mode, if a touch event or
     *     button press is detected, the Pinnacle ASIC will take about 300
     *     milliseconds to wake up (does not include handling the touch event or
     *     button press data). Remember that releasing a held button is also
     *     considered an input event.
     *
     * @param isEnabled ``true`` if you want the Pinnacle ASIC to enter sleep
     *     (low power) mode after about 5 seconds of inactivity (does not apply to
     *     AnyMeas mode). ``false`` if you don't want the Pinnacle ASIC to enter
     *     sleep mode.
     */
    void allowSleep(bool isEnabled);
    /**
     * This function describes if the Pinnacle ASIC is configured to enter
     * sleep mode. This does not apply to AnyMeas mode.
     *
     * @returns
     *     The setting configured by `allowSleep()` or ``false`` if `begin()`
     *     failed to initialize the trackpad.
     */
    bool isAllowSleep();
    /**
     * This function controls power state of the Pinnacle ASIC that drives the
     * trackpad.
     *
     * .. note:: The ASIC will take about 300 milliseconds to complete the
     *     transition from powered down mode to active mode. No touch events or
     *     button presses will be monitored while powered down.
     *
     * @param isOff ``true`` means power down (AKA standby mode), and ``false``
     *     means power up (Active, Idle, or Sleep mode).
     */
    void shutdown(bool isOff);
    /**
     * This function describes if the Pinnacle ASIC is in a power down mode
     * or not.
     *
     * @returns The setting configured by `shutdown()`
     */
    bool isShutdown();
    /**
     * This function controls how many samples (of data) per second are
     * taken. This function only applies to `PINNACLE_RELATIVE` or
     * `PINNACLE_ABSOLUTE` mode, otherwise if `setDataMode()` is given
     * `PINNACLE_ANYMEAS`, then this function will do nothing.
     *
     * @param value Valid input values are ``100``, ``80``, ``60``, ``40``, ``20``,
     *     ``10``. Any other input values automatically set the sample rate to 100
     *     sps (samples per second). Optionally, ``200`` and ``300`` sps can be
     *     specified, but using these optional values automatically disables palm
     *     (referred to as "NERD" in the specification sheet) and noise
     *     compensations. These higher values are meant for using a stylus with a
     *     2mm diameter tip, while the values less than 200 are meant for a finger
     *     or stylus with a 5.25mm diameter tip.
     */
    void setSampleRate(uint16_t value);
    /**
     * This function describes the sample rate that the Pinnacle ASIC uses for
     * reporting data.
     *
     * :Return:
     *     The setting configured by `setSampleRate()` or ``0`` if `setDataMode()` is
     *     given `PINNACLE_ANYMEAS`.
     */
    uint16_t getSampleRate();
    /**
     * This function will configure the Pinnacle ASIC to detect either finger,
     * stylus, or both.
     *
     * .. tip:: Consider adjusting the ADC matrix's gain to enhance
     *     performance/results using `setAdcGain()`.
     *
     * @param enableFinger ``true`` enables the Pinnacle ASIC's measurements to
     *     detect if the touch event was caused by a finger or 5.25mm stylus.
     *     ``false`` disables this feature. Default is ``true``.
     * @param enableStylus ``true`` enables the Pinnacle ASIC's measurements to
     *     detect if the touch event was caused by a 2mm stylus. ``false`` disables
     *     this feature. Default is ``true``.
     * @param sampleRate See the setSampleRate() as this parameter directly
     *     calls that function.
     */
    void detectFingerStylus(bool enableFinger = true,
                            bool enableStylus = true,
                            uint16_t sampleRate = 100);
    /**
     * Set calibration parameters when the Pinnacle ASIC calibrates itself.
     * This function only applies to `PINNACLE_RELATIVE` or
     * `PINNACLE_ABSOLUTE` mode, otherwise if `setDataMode()` is given
     * `PINNACLE_ANYMEAS`, then this function will do nothing.
     *
     * .. note:: According to the datasheet, calibration of the sensor takes about
     *     100 milliseconds. This function will block until calibration is complete
     *     (if ``run`` is ``true``). It is recommended for typical applications to
     *     leave all optional parameters in their default states.
     *
     * @param run If ``true``, this function forces a calibration of the sensor.
     *     If ``false``, this function just writes the following parameters to the
     *     Pinnacle ASIC's "CalConfig1" register.
     * @param tap Enable dynamic tap compensation? Default is ``true``.
     * @param trackError Enable dynamic track error compensation? Default is
     *     ``true``.
     * @param nerd Enable dynamic NERD compensation? Default is ``true``. This
     *     parameter has something to do with palm detection/compensation.
     * @param background Enable dynamic background compensation? Default is
     *     ``true``.
     * @returns
     *     ``false``
     *         - If `setDataMode()` is not set to `PINNACLE_RELATIVE` or `PINNACLE_ABSOLUTE`.
     *         - If the calibration `run` timed out after 100 milliseconds.
     *     ``true``
     *         - If `setDataMode()` is not given `PINNACLE_RELATIVE` or `PINNACLE_ABSOLUTE` and the
     *           calibration is not `run`.
     *         - If the calibration `run` successfully finishes.
     */
    bool calibrate(bool run = true,
                   bool tap = true,
                   bool trackError = true,
                   bool nerd = true,
                   bool background = true);
    /**
     * Manually sets the compensation matrix (array) of the 46 16-bit unsigned
     * integer values stored in the Pinnacle ASIC's memory that is used for
     * taking measurements. This matrix is not applicable to `PINNACLE_ANYMEAS` mode.
     *
     * @param matrix The array of 16-bit unsigned integers that will be used
     *     for compensation calculations when measuring of input events.
     * @param len The length of the array passed to the `matrix` parameter.
     *     Default is 46 (the maximum elements used).
     *
     * .. seealso:: Review the hint in `getCalibrationMatrix()` from the Pinnacle
     *     ASIC's application note about deciding what values to use.
     */
    void setCalibrationMatrix(int16_t* matrix, uint8_t len = 46);
    /**
     * Use this function to compare a prior compensation matrix with a new
     * matrix that was either loaded manually via `setCalibrationMatrix()` or
     * created internally by calling `calibrate()` with the ``run`` parameter as
     * ``true``.
     *
     * .. hint:: A note from Cirque's Application Note on Comparing matrices:
     *
     *     If any 16-bit values are above 20K (absolute), it generally
     *     indicates a problem with the sensor. If no values exceed 20K,
     *     proceed with the data comparison. Compare each 16-bit value in one
     *     matrix to the corresponding 16-bit value in the other matrix. If
     *     the difference between the two values is greater than 500
     *     (absolute), it indicates a change in the environment. Either an
     *     object was on the sensor during calibration, or the surrounding
     *     conditions (temperature, humidity, or noise level) have changed.
     *     One strategy is to force another calibration and compare again, if
     *     the values continue to differ by 500, determine whether to use the
     *     new data or a previous set of stored data. Another strategy is to
     *     average any two values that differ by more than 500 and write this
     *     new matrix, with the average values, back into Pinnacle ASIC.
     *
     * @param[out] matrix A reference pointer (declared array of 46 16-bit unsigned
     *     integers) for storing the compensation matrix configured by
     *     `setCalibrationMatrix()` or created internally by `calibrate()` (or after a
     *     "power-on-reset" condition).
     */
    void getCalibrationMatrix(int16_t* matrix);
    /**
     * Sets the ADC (Analog to Digital Converter) attenuation (gain ratio) to
     * enhance performance based on the overlay type. This does not apply to
     * AnyMeas mode. However, the input value specified can be written while
     * `setDataMode()` is given `PINNACLE_ANYMEAS`, but there is no guarantee that
     * it will "stick" as it may be overridden by the Pinnacle ASIC
     * (specification sheet does not imply either way).
     *
     * @param sensitivity This byte specifies how sensitive the ADC component
     *     is. It must be in range [0, 3]. Where ``0`` means most sensitive, and ``3``
     *     means least sensitive. A value outside this range will default to ``0``.
     *
     * .. tip:: The official example code from Cirque for a curved overlay uses a
     *     value of ``1``.
     */
    void setAdcGain(uint8_t sensitivity);
    /**
     * According to the comments in the official example code from Cirque,
     * "Changes thresholds to improve detection of fingers." This function was
     * ported from Cirque's example code and doesn't have corresponding
     * documentation. Thus, the defaults for this function's parameters use
     * the same values found in the official example. I'm unaware of any
     * documented memory map for the Pinnacle ASIC as this function directly
     * alters values in the Pinnacle ASIC's memory. ALTER THESE PARAMETERS AT
     * YOUR OWN RISK!
     */
    void tuneEdgeSensitivity(uint8_t xAxisWideZMin = 4, uint8_t yAxisWideZMin = 3);
#if PINNACLE_ANYMEAS_SUPPORT
    /**
     * This function configures the Pinnacle ASIC for taking raw ADC
     * measurements. Be sure that `setDataMode()` is given
     * `PINNACLE_ANYMEAS` before calling this function otherwise it will do
     * nothing.
     *
     * .. note:: The `apertureWidth` parameter has an inverse relationship/affect
     *     on the `frequency` parameter. The approximated frequencies described
     *     in this documentation are based on an aperture width of 500
     *     nanoseconds, and they will shrink as the aperture width grows or grow
     *     as the aperture width shrinks.
     *
     * @param gain Sets the sensitivity of the ADC matrix. Valid values are the
     *     constants defined in `PinnacleAnyMeasGain`. Defaults to `PINNACLE_GAIN_200`.
     * @param frequency Sets the frequency of measurements made by the ADC
     *     matrix. Valid values are the constants defined in
     *     `PinnacleAnyMeasFreq`. Defaults to `PINNACLE_FREQ_0`.
     * @param sampleLength Sets the maximum bit length of the measurements made
     *     by the ADC matrix. Valid values are ``128``, ``256``, or ``512``. Defaults to ``512``.
     * @param muxControl The Pinnacle ASIC can employ different bipolar
     *     junctions and/or reference capacitors. Valid values are the constants
     *     defined in `PinnacleAnyMeasMuxing`. Additional combination of these
     *     constants is also allowed. Defaults to `PINNACLE_MUX_PNP`.
     * @param apertureWidth Sets the window of time (in nanoseconds) to allow
     *     for the ADC to take a measurement. Valid values are multiples of 125 in
     *     range [``250``, ``1875``]. Erroneous values are clamped/truncated to this range.
     * @param controlPowerCount Configure the Pinnacle to perform a number of
     *     measurements for each call to `measureAdc()`. Defaults to 1. Constants
     *     defined in `PinnacleAnyMeasCtrl` can be added (with ``+``) to specify if
     *     sleep is allowed (`PINNACLE_CRTL_PWR_IDLE` -- this is not default) or
     *     if repetitive measurements is allowed (`PINNACLE_CRTL_REPEAT`) when
     *     number of measurements is more than 1.
     *
     *     .. warning:: There is no bounds checking on the number of measurements
     *         specified here. Specifying more than 63 will trigger sleep mode after
     *         performing measurements.
     *     .. hint:: Be aware that allowing the Pinnacle to enter sleep mode after
     *         taking measurements will cause a latency in consecutive calls to
     *         `measureAdc()` as the Pinnacle requires about 300 milliseconds to wake up.
     */
    void anymeasModeConfig(uint8_t gain = PINNACLE_GAIN_200,
                           uint8_t frequency = PINNACLE_FREQ_0,
                           uint32_t sampleLength = 512,
                           uint8_t muxControl = PINNACLE_MUX_PNP,
                           uint32_t apertureWidth = 500,
                           uint8_t controlPowerCount = 1);
    /**
     * This function instigates and returns the measurement (a signed short
     * integer) from the Pinnacle ASIC's ADC (Analog to Digital Converter)
     * matrix (only applies to AnyMeas mode).
     *
     * Internally, this function uses the non-blocking helper functions
     * `startMeasureAdc()` and `getMeasureAdc()`, but blocks until ADC measurements are
     * completed. Be sure that `setDataMode()` is given `PINNACLE_ANYMEAS` before
     * calling this function otherwise it will do nothing and return ``0``.
     *
     * :Return:
     *     A signed short integer. If `setDataMode()` is not given
     *     `PINNACLE_ANYMEAS`, then this function returns ``0`` and does nothing.
     * :4-byte Integer Format (for use as each parameter):
     *     Bits 29 and 28 represent the optional implementation of reference
     *     capacitors built into the Pinnacle ASIC. To use these capacitors, the
     *     corresponding constants (`PINNACLE_MUX_REF0` and/or
     *     `PINNACLE_MUX_REF1`) must be passed to `anymeasModeConfig()` in the
     *     ``muxControl`` parameter, and their representative bits must be flagged
     *     in both the `bitsToToggle` and `togglePolarity` parameters.
     *
     *     .. csv-table:: byte 3 (MSByte)
     *           :stub-columns: 1
     *           :widths: 10, 5, 5, 5, 5, 5, 5, 5, 5
     *
     *           "bit position",31,30,29,28,27,26,25,24
     *           "representation",N/A,N/A,Ref1,Ref0,Y11,Y10,Y9,Y8
     *     .. csv-table:: byte 2
     *           :stub-columns: 1
     *           :widths: 10, 5, 5, 5, 5, 5, 5, 5, 5
     *
     *           "bit position",23,22,21,20,19,18,17,16
     *           "representation",Y7,Y6,Y5,Y4,Y3,Y2,Y1,Y0
     *     .. csv-table:: byte 1
     *           :stub-columns: 1
     *           :widths: 10, 5, 5, 5, 5, 5, 5, 5, 5
     *
     *           "bit position",15,14,13,12,11,10,9,8
     *           "representation",X15,X14,X13,X12,X11,X10,X9,X8
     *     .. csv-table:: byte 0 (LSByte)
     *           :stub-columns: 1
     *           :widths: 10, 5, 5, 5, 5, 5, 5, 5, 5
     *
     *           "bit position",7,6,5,4,3,2,1,0
     *           "representation",X7,X6,X5,X4,X3,X2,X1,X0
     *
     * .. seealso:: Review the :ref:`anymeas_mode.ino example <arduino_anymeas_example>` to
     *     understand how to use these
     *
     * @param bitsToToggle This 4-byte integer specifies which bits the
     *     Pinnacle touch controller should toggle. A bit of ``1`` flags that bit
     *     for toggling, and a bit of ``0`` signifies that the bit should remain
     *     unaffected.
     * @param togglePolarity This 4-byte integer specifies which polarity the
     *     specified bits (from `bitsToToggle` parameter) are toggled. A bit of
     *     ``1`` toggles that bit positive, and a bit of ``0`` toggles that bit
     *     negative.
     */
    int16_t measureAdc(unsigned int bitsToToggle, unsigned int togglePolarity);
    /**
     * A non-blocking function to instigate ADC measurements when the `setDataMode()` is given
     * `PINNACLE_ANYMEAS` mode. See parameters and table in `measureAdc()` as
     * this helper function's parameters are used exactly the same.
     */
    void startMeasureAdc(unsigned int bitsToToggle, unsigned int togglePolarity);
    /**
     * A non-blocking function (meant to be used in conjunction with
     * `startMeasureAdc()`) to retrieve the result of ADC measurements based on
     * parameters passed to `startMeasureAdc()`. Be sure that `available()` returns ``true``
     * before calling this function because it will clear status flags (used by `available()`).
     *
     * :Returns:
     *
     *     - A 16-bit integer if `setDataMode()` is given `PINNACLE_ANYMEAS`.
     *     - ``0`` if `setDataMode()` is not given `PINNACLE_ANYMEAS`.
     */
    int16_t getMeasureAdc();
#endif // PINNACLE_ANYMEAS_SUPPORT == true

#if PINNACLE_DEV_HW_DEBUG
    // A handy function to get a register dump from the sensor.
    void readRegisters(uint8_t reg, uint8_t* data, uint8_t len);
#endif

private:
    void eraWrite(uint16_t, uint8_t);
    void eraWriteBytes(uint16_t, uint8_t, uint8_t);
    void eraRead(uint16_t, uint8_t*);
    void eraReadBytes(uint16_t, uint8_t*, uint8_t);
    PinnacleDataMode _dataMode;
    bool _intellimouse;
    const pinnacle_gpio_t _dataReady;
    virtual void rapWriteCmd(uint8_t*, uint8_t) = 0;
    virtual void rapWrite(uint8_t, uint8_t) = 0;
    virtual void rapWriteBytes(uint8_t, uint8_t*, uint8_t) = 0;
    virtual void rapRead(uint8_t, uint8_t*) = 0;
    virtual void rapReadBytes(uint8_t, uint8_t*, uint8_t) = 0;

protected:
    /**
     * Starts the driver interface on the appropriate data bus.
     *
     * :Return:
     *     - ``true`` if the Pinnacle ASIC was setup and configured properly (with data
     *       feed enabled using Relative mode).
     *     - ``false`` if the Pinnacle ASIC was unresponsive for some reason; all further
     *       operations will be nullified by `setDataMode()` to `PINNACLE_ERROR` (``0xFF``).
     */
    bool begin();
};

/**
 * Derived class for interfacing with the Pinnacle ASIC via the SPI protocol.
 *
 * @ingroup pinnacle-touch-api
 */
class PinnacleTouchSPI : public PinnacleTouch
{
public:
    /**
     * Create an instance to interface with the Pinnacle ASIC over an SPI bus.
     *
     * @param dataReadyPin The input pin connected to the Pinnacle ASIC's "Data Ready" pin.
     *     To use the Pinnacle's Software Data Ready flag instead, set this to `PINNACLE_SW_DR`.
     *     In general, it recommended to use the Hardware Data Ready pin for speed and efficient
     *     bus activity.
     *
     *     .. anymeas-limits::
     *
     *         A Hardware Data Ready pin is required for anymeas mode.
     * @param slaveSelectPin The "slave select" pin output to the Pinnacle ASIC.
     * @param spiSpeed The speed (or baudrate) to use on the SPI bus.
     *     Defaults to the Pinnacle ASIC's maximum 13 MHz (``13000000``).
     */
    PinnacleTouchSPI(pinnacle_gpio_t dataReadyPin, pinnacle_gpio_t slaveSelectPin, uint32_t spiSpeed = PINNACLE_SPI_SPEED);
    /**
     * Starts the driver interface on the appropriate SPI bus.
     *
     * @returns The same value as `PinnacleTouch::begin()`.
     */
    bool begin();
    /**
     * Starts the driver interface on the specified SPI bus object. This function is useful if the
     * application uses more than one SPI bus.
     *
     * @param spi_bus A reference to the instantiated object that corresponds to
     *     a specific SPI bus.
     *
     * @returns The same value as `PinnacleTouch::begin()`.
     *
     * @id pinnacle_spi_t
     */
    bool begin(pinnacle_spi_t* spi_bus);

private:
    void rapWriteCmd(uint8_t*, uint8_t);
    void rapWrite(uint8_t, uint8_t);
    void rapWriteBytes(uint8_t, uint8_t*, uint8_t);
    void rapRead(uint8_t, uint8_t*);
    void rapReadBytes(uint8_t, uint8_t*, uint8_t);
    const pinnacle_gpio_t _slaveSelect;
    const uint32_t _spiSpeed;
    pinnacle_spi_t* spi;
};

/**
 * Derived class for interfacing with the Pinnacle ASIC via the I2C protocol.
 *
 * @ingroup pinnacle-touch-api
 */
class PinnacleTouchI2C : public PinnacleTouch
{
public:
    /**
     * Create an instance to interface with the Pinnacle ASIC over an I2C bus.
     *
     * @param dataReadyPin The input pin connected to the Pinnacle ASIC's "Data Ready" pin.
     *     To use the Pinnacle's Software Data Ready flag instead, set this to `PINNACLE_SW_DR`.
     *     In general, it recommended to use the Hardware Data Ready pin for speed and efficient
     *     bus activity.
     *
     *     .. anymeas-limits::
     *
     *         A Hardware Data Ready pin is required for anymeas mode.
     * @param slaveAddress The slave I2C address of the Pinnacle ASIC. Defaults to ``0x2A``.
     */
    PinnacleTouchI2C(pinnacle_gpio_t dataReadyPin, uint8_t slaveAddress = 0x2A);
    /**
     * Starts the driver interface on the appropriate I2C bus.
     *
     * @returns The same value as `PinnacleTouch::begin()`.
     */
    bool begin();
    /**
     * Starts the driver interface on the specified I2C bus object. This function is useful if the
     * application uses more than one I2C bus.
     *
     * @param i2c_bus A reference to the instantiated object that corresponds to
     *     a specific I2C bus.
     *
     * @returns The same value as `PinnacleTouch::begin()`.
     *
     * @id pinnacle_i2c_t
     */
    bool begin(pinnacle_i2c_t* i2c_bus);

private:
    void rapWriteCmd(uint8_t*, uint8_t);
    void rapWrite(uint8_t, uint8_t);
    void rapWriteBytes(uint8_t, uint8_t*, uint8_t);
    void rapRead(uint8_t, uint8_t*);
    void rapReadBytes(uint8_t, uint8_t*, uint8_t);
    const uint8_t _slaveAddress;
    pinnacle_i2c_t* i2c;
};

#endif // _CIRQUEPINNACLE_H_
