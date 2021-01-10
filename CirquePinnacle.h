/**
 *  @file CirquePinnacle.h
 *  A library to interface (via I2C or SPI protocol) with Cirque's Glidepoint circle
 *  touchpads that employ Cirque's Pinnacle ASIC touch controller (1CA027).
 *
 *  Store links (where to buy):
 *  - [Individual trackpads](https://www.mouser.com/Search/Refine?Ntk=P_MarCom&Ntt=118816186)
 *  - [developer kits](https://www.mouser.com/Search/Refine?Ntk=P_MarCom&Ntt=183712866)
 *
 *  Written by Brendan Doherty to include all functionality demonstrated by
 *  the official Cirque github repository based on the teensy3.2 and the
 *  Cirque Glidepoint circle trackpad developer kit.
 *  - [Cirque example repository](https://github.com/cirque-corp/Cirque_Pinnacle_1CA027)
 *
 *  License and copyright information is located at this repository's root
 *  directory under LICENSE
 */
#ifndef CirquePinnacle_H
#define CirquePinnacle_H
#include <stdint.h>

/**
 * @defgroup RegisterOffsets Pinacle Register Addresses
 * @brief defined constants for Pinnacle registers
 * @{
 */
#define PINNACLE_FIRMWARE_ID       0x00
#define PINNACLE_STATUS            0x02
#define PINNACLE_SYS_CONFIG        0x03
#define PINNACLE_FEED_CONFIG_1     0x04
#define PINNACLE_FEED_CONFIG_2     0x05
#define PINNACLE_FEED_CONFIG_3     0x06
#define PINNACLE_CAL_CONFIG        0x07
#define PINNACLE_SAMPLE_RATE       0x09
#define PINNACLE_Z_IDLE            0x0A
#define PINNACLE_Z_SCALER          0x0B
#define PINNACLE_SLEEP_INTERVAL    0x0C  // time of sleep until checking for finger
#define PINNACLE_SLEEP_TIMER       0x0D  // time after idle mode until sleep starts
#define PINNACLE_PACKET_BYTE_0     0x12
#define PINNACLE_PACKET_BYTE_1     0x13
#define PINNACLE_ERA_VALUE         0x1B
#define PINNACLE_ERA_ADDR          0x1C
#define PINNACLE_ERA_CONTROL       0x1E
#define PINNACLE_HCO_ID            0x1F


//*************** defined Constants for bitwise configuration*****************
/**
 * @}
 * Allowed symbols for configuring the Pinanacle ASIC's data
 * reporting/measurements.
 * @rst
 * .. seealso:: `~PinnacleTouch::getDataMode()`, `~PinnacleTouch::setDataMode()`
 * @endrst
 */
enum PinnacleDataMode
{
    /** Alias symbol for specifying Relative mode (AKA Mouse mode). */
    PINNACLE_RELATIVE = 0x00,
    /** Alias symbol for specifying "AnyMeas" mode (raw ADC measurement) */
    PINNACLE_ANYMEAS = 0x01,
    /** Alias symbol for specifying Absolute mode (axis positions) */
    PINNACLE_ABSOLUTE = 0x02,
};

/**
 * Allowed ADC gain configurations of AnyMeas mode.
 *
 * The percentages defined here are approximate values.
 * @rst
 * .. seealso:: `~PinnacleTouch::anyMeasModeConfig()`
 * @endrst
 */
enum PinnacleAnyMeasGain
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
 * width of 500 nanoseconds. If the `apertureWidth` parameter to
 * PinnacleTouch::anyMeasModeConfig() specified is less than 500 nanoseconds,
 * then the frequency will be larger than what is described here (& vice
 * versa).
 * @rst
 * .. seealso:: `~PinnacleTouch::anyMeasModeConfig()`
 * @endrst
 */
enum PinnacleAnyMeasFreq
{
    /** frequency around 500,000Hz */
    PINNACLE_FREQ_0 = 0x02,
    /** frequency around 444,444Hz */
    PINNACLE_FREQ_1 = 0x03,
    /** frequency around 400,000Hz */
    PINNACLE_FREQ_2 = 0x04,
    /** frequency around 363,636Hz */
    PINNACLE_FREQ_3 = 0x05,
    /** frequency around 333,333Hz */
    PINNACLE_FREQ_4 = 0x06,
    /** frequency around 307,692Hz */
    PINNACLE_FREQ_5 = 0x07,
    /** frequency around 267,000Hz */
    PINNACLE_FREQ_6 = 0x09,
    /** frequency around 235,000Hz */
    PINNACLE_FREQ_7 = 0x0B,
};

/**
 * Allowed muxing gate polarity and reference capacitor configurations
 * of AnyMeas mode.
 *
 * Combining these values (with `+` operator) is allowed.
 * @rst
 * .. note:: The sign of the measurements taken in AnyMeas mode is inverted
 *     depending on which muxing gate is specified (when specifying an individual
 *     gate polarity).
 * .. seealso:: `~PinnacleTouch::anyMeasModeConfig()`, `~PinnacleTouch::measureAdc()`
 * @endrst
 */
enum PinnacleAnyMeasMuxing
{
    /**
     * enables a builtin capacitor (~0.5pF).
     * @rst
     * .. seealso:: `~PinnacleTouch::measureAdc()`
     * @endrst
     */
    PINNACLE_MUX_REF1 = 0x10,
    /**
     * enables a builtin capacitor (~0.25pF).
     * @rst
     * .. seealso:: `~PinnacleTouch::measureAdc()`
     * @endrst
     */
    PINNACLE_MUX_REF0 = 0x08,
    /**
     * enable PNP sense line
     */
    PINNACLE_MUX_PNP = 0x04,
    /**
     * enable NPN sense line
     */
    PINNACLE_MUX_NPN = 0x01,
};

/**
 * These constants control the number of measurements performed in
 * PinnacleTouch::measureADC().
 * @rst
 * .. important:: The number of measurements can range [0, 63].
 * .. seealso:: `~PinnacleTouch::anyMeasModeConfig()`
 * @endrst
 */
enum PinnacleAnyMeasCtrl
{
    /**
     * only required for more than 1 measurement
     */
    PINNACLE_CRTL_REPEAT = 0x80,
    /**
     * triggers low power mode (sleep) after completing measurements
     */
    PINNACLE_CRTL_PWR_IDLE = 0x40,
};

/**
 * @rst
 * This data structure used for returning data reports in relative mode using
 * `read() for Relative Mode`_.
 * @endrst
 */
struct RelativeReport
{
    /**
     * @brief This will always be in range [0, 7].
     *
     * The returned button data is a byte in which each bit
     * represents if a button is pressed. The bit to button order is as
     * follows:
     * - 0. [LSB] Button 1 (thought of as Left mouse button). If `allTaps`
     *      parameter is passed as `true` when calling
     *      PinnacleTouch::relativeModeConfig(), a single tap will be
     *      reflected here.
     * - 1. Button 2 (thought of as Right mouse button). If `allTaps` and
     *      `secondaryTap` parameters are passed as `true` when calling
     *      PinnacleTouch::relativeModeConfig(), a single tap in the perspective
     *      top-left-most corner will be reflected here (secondary taps are
     *      constantly disabled if isHardConfigured() returns `true`). Note
     *      that the top-left-most corner can be perspectively moved if
     *      `rotate90` parameter is passed as `true` when calling
     *      PinnacleTouch::relativeModeConfig().
     * - 2. Button 3 (thought of as Middle mouse or scroll wheel button)
     */
    uint8_t buttons;
    /**
     * @brief This will always be in range -128 <= `x` <= 127
     */
    int8_t x;
    /**
     * @brief This will always be in range -128 <= `y` <= 127
     */
    int8_t y;
    /**
     * @brief This will always be in range -128 <= `scroll` <= 127
     *
     * @rst
     * .. note:: In Relative/Mouse mode the scroll wheel data is only reported
     *     if the ``intellimouse`` parameter is passed as ``true`` to
     *     `~PinnacleTouch::relativeModeConfig()`. Otherwise this is an empty byte as
     *     the `RelativeReport` follows the buffer structure of a mouse HID report.
     * @endrst
     */
    int8_t scroll;
};

/**
 * @rst
 * This data structure used for returning data reports in absolute mode using
 * `read() for Absolute Mode`_.
 * @endrst
 */
struct AbsoluteReport
{
    /**
     * @brief This will always be in range [0, 7].
     *
     * The returned button data is a byte in which each bit
     * represents a button. The bit to button order is as follows:
     * @rst
     * .. csv-table::
     *     :header: "bit position", "button number"

     *     "0 (LSB)", "Button 1"
     *     1, "Button 2"
     *     2, "Button 3"
     * @endrst
     */
    uint8_t buttons;
    /**
     * @brief This will always be in range 0 <= `x` <= 2047
     * @rst
     * .. tip:: The datasheet recommends this value should be
     *     clamped to range 128 <= `x` <= 1920 for reliability.
     * @endrst
     */
    uint16_t x;
    /**
     * @brief This will always be in range 0 <= `y` <= 1535
     * @rst
     * .. tip:: The datasheet recommends this value should be
     *     clamped to range 64 <= `y` <= 1472 for reliability.
     * @endrst
     */
    uint16_t y;
    /**
     * @brief This will always be in range 0 <= `z` <= 65535. The maximum
     * value will depend on senitivity.
     */
    uint8_t z;
};

/**
 * The abstract base class for driving the Pinnacle ASIC.
 */
class PinnacleTouch{
public:
    /**
     * @brief Create an instance to use as an interface with the Pinnacle ASIC
     * touch controller.
     * @param dataReadyPin The input pin connected to the Pinnacle ASIC's
     * "Data Ready" pin.
     */
    PinnacleTouch(uint8_t);
    /**
     * @rst
     * This function controls if the touch/button event data is reported or
     * not. It only applies to `PINNACLE_RELATIVE` or `PINNACLE_ABSOLUTE`
     * mode, otherwise if `Data Mode`_ is set to `PINNACLE_ANYMEAS`, then this
     * function will do nothing.
     * @endrst
     * @param isEnabled Enables (`true`) or disables (`false`) data reporting.
     */
    void feedEnabled(bool);
    /**
     * This function describes if the touch/button event data is to be
     * reported or not.
     * @rst
     * :Return:
     *     The setting configured by `feedEnabled()` or ``false`` if `Data Mode`_
     *     is set to `PINNACLE_ANYMEAS`.
     * @endrst
     */
    bool isFeedEnabled();
    /**
     * This function controls which mode the data report is configured for.
     * @param mode Valid input values are
     * @rst
     * .. csv-table::
     *     :header: "``mode`` (enum value)", description
     *
     *     "`PINNACLE_RELATIVE` (``0``)", "for relative/mouse mode"
     *     "`PINNACLE_ANYMEAS` (``1``)", "for reading raw ADC values"
     *     "`PINNACLE_ABSOLUTE` (``2``)", "for absolute positioning mode"
     *
     * Invalid input values have no affect.
     * @endrst
     */
    void setDataMode(uint8_t);
    /**
     * This function describes which mode the data report is configured for.
     * @rst
     * .. important::
     *     When switching from `PINNACLE_ANYMEAS` to `PINNACLE_RELATIVE`
     *     or `PINNACLE_ABSOLUTE` all configurations are reset, and must be
     *     re-configured by using  `absoluteModeConfig()` or `relativeModeConfig()`.
     * @endrst
     * @returns
     * - `0` (AKA @ref PINNACLE_RELATIVE) for Relative mode (AKA mouse mode)
     * - `1` (AKA @ref PINNACLE_ANYMEAS) for AnyMeas mode (raw ADC measurements)
     * - `2` (AKA @ref PINNACLE_ABSOLUTE) for Absolute mode (X & Y axis
     *   positions)
     * - `255` if begin() returns `false` (failed to initialize the trackpad)
     */
    uint8_t getDataMode();
    /**
     * @rst
     * This function can be used to inform applications about the factory
     * customized hardware configuration.
     *
     * .. seealso:: Read the section about product labeling in
     *     `Model Labeling Scheme &lt;index.html#model-labeling-scheme&gt;`_.
     * @endrst
     * @returns `true` if a 470K ohm resistor is populated at the junction
     * labeled "R4"; `false` if no resistor is populated at the "R4" junction.
     * This function will also return `false` if begin() failed to initialize
     * the trackpad.
     */
    bool isHardConfigured();
    /**
     * @rst
     * Use this function to detirmine if there is new data to report.
     * Internally, this function checks if the interrupt signal on the "data
     * ready" pin (labeled "DR" in the `pinout &lt;index.html#pinout&gt;`_ section)
     * is active. Data (new or antiquated) can be retreived using
     * `read()` or `read()` depending on what `Data Mode`_ is set
     * to.
     * @endrst
     * @returns `true` if there is new data to report; `false` if there is no
     * new data to report.
     */
    bool available();
    /**
     * @rst
     * Configure settings specific to Absolute mode (reports axis positions). This function only
     * applies to `PINNACLE_ABSOLUTE` mode, otherwise if `Data Mode`_ is set to
     * `PINNACLE_ANYMEAS` or `PINNACLE_RELATIVE`, then this function does nothing.
     * @endrst
     * @param zIdleCount Specifies the number of empty packets (x-axis, y-axis, and z-axis
     * are `0`) reported (every 10 milliseconds) when there is no touch detected. Defaults
     * to 30. This number is clamped to range [0, 255].
     * @param invertX Specifies if the x-axis data is to be inverted before reporting it.
     * Default is `false`.
     * @param invertY Specifies if the y-axis data is to be inverted before reporting it.
     * Default is `false`.
     */
    void absoluteModeConfig(uint8_t zIdleCount=30,
                            bool invertX=false,
                            bool invertY=false);
    /**
     * @rst
     * Configure settings specific to Relative mode (AKA Mouse mode) data reporting. This function
     * only applies to `PINNACLE_RELATIVE` mode, otherwise if `Data Mode`_ is set to
     * `PINNACLE_ANYMEAS` or `PINNACLE_ABSOLUTE`, then this function does nothing.
     * @endrst
     * @param rotate90 Specifies if the axis data is altered for 90 degree rotation before
     * reporting it (essentially swaps the axis data). Default is `false`.
     * @param allTaps Specifies if all taps should be reported (`true`) or not
     * (`false`). Default is `true`. This affects `secondaryTap` option as well.
     * @param secondaryTap Specifies if tapping in the top-left corner (depending on
     * orientation) triggers the secondary button data. Defaults to `true`. This feature is
     * always disabled if isHardConfigured() is `true`.
     * @param glideExtend A patended feature that allows the user to glide their finger off
     * the edge of the sensor and continue gesture with the touch event. Default is `true`.
     * This feature is always disabled if isHardConfigured() is `true`.
     * @param intellimouse Specifies if the data reported includes a byte about scroll data.
     * Default is `false`. This feature is always disabled if isHardConfigured()
     * is `true`.
     */
    void relativeModeConfig(bool rotate90=false,
                            bool allTaps=true,
                            bool secondaryTap=true,
                            bool glideExtend=false,
                            bool intellimouse=false);
    /**
     * @rst
     * This function will fetch touch (and button) event data from the
     * Pinnacle ASIC. This function only applies to `PINNACLE_RELATIVE` mode,
     * otherwise if `Data Mode`_ is set to `PINNACLE_ANYMEAS` or
     * `PINNACLE_ABSOLUTE`, then this function does nothing.
     * @endrst
     * @param[out] report A reference pointer (declared variable of datatype
     * RelativeReport) for storing the data that describes the touch (and
     * button) event.
     */
    void read(RelativeReport* report);
    /**
     * @rst
     * This function will fetch touch (and button) event data from the
     * Pinnacle ASIC (including empty packets on ending of a touch/button
     * event). This function only applies to `PINNACLE_ABSOLUTE` mode, otherwise
     * if `Data Mode`_ is set to `PINNACLE_ANYMEAS` or `PINNACLE_RELATIVE`,
     * then this function does nothing.
     * @endrst
     * @param[out] report A reference pointer (declared variable of datatype
     * AbsoluteReport) for storing the data that describes the touch (and
     * button) event.
     */
    void read(AbsoluteReport* report);
    /**
     * @rst
     * Use this function to clear the interrupt signal (digital input; active
     * when HIGH) on the "data ready" pin (marked "DR" in the
     * `pinout &lt;index.html#pinout&gt;`_ section). This function is mainly used
     * internally when applicable, but it is left exposed if the application
     * wants to neglect a data report when desirable.
     * @endrst
     */
    void clearStatusFlags();
    /**
     * This will specify if the Pinnacle ASIC is allowed to sleep after about
     * 5 seconds of idle activity (no input event).
     * @rst
     * .. note:: While the touch controller is in sleep mode, if a touch event or
     *     button press is detected, the Pinnacle ASIC will take about 300
     *     milliseconds to wake up (does not include handling the touch event or
     *     button press data). Remember that releasing a held button is also
     *     considered an input event.
     * @endrst
     * @param isEnabled `true` if you want the Pinnacle ASIC to enter sleep
     * (low power) mode after about 5 seconds of inactivity (does not apply to
     * AnyMeas mode). `false` if you don't want the Pinnacle ASIC to enter
     * sleep mode.
     */
    void allowSleep(bool);
    /**
     * This function describes if the Pinnacle ASIC is configured to enter
     * sleep mode. This does not apply to AnyMeas mode.
     * @returns The setting configured by allowSleep() or `false` if begin()
     * failed to initialize the trackpad.
     */
    bool isAllowSleep();
    /**
     * This function controls power state of the Pinnacle ASIC that drives the
     * touchpad.
     * @rst
     * .. note:: The ASIC will take about 300 milliseconds to complete the
     *     transition from powered down mode to active mode. No touch events or
     *     button presses will be monitored while powered down.
     * @endrst
     * @param isOff `true` means power down (AKA standby mode), and `false`
     * means power up (Active, Idle, or Sleep mode).
     */
    void shutdown(bool);
    /**
     * This function describes if the Pinnacle ASIC is in a power down mode
     * or not.
     * @returns The setting configured by shutdown()
     */
    bool isShutdown();
    /**
     * @rst
     * This function controls how many samples (of data) per second are
     * taken. This function only applies to `PINNACLE_RELATIVE` or
     * `PINNACLE_ABSOLUTE` mode, otherwise if `Data Mode`_ is set to
     * `PINNACLE_ANYMEAS`, then this function will do nothing.
     * @endrst
     * @param value Valid input values are `100`, `80`, `60`, `40`, `20`,
     * `10`. Any other input values automatically set the sample rate to 100
     * sps (samples per second). Optionally, `200` and `300` sps can be
     * specified, but using these optional values automatically disables palm
     * (referred to as "NERD" in the specification sheet) and noise
     * compensations. These higher values are meant for using a stylus with a
     * 2mm diameter tip, while the values less than 200 are meant for a finger
     * or stylus with a 5.25mm diameter tip.
     */
    void setSampleRate(uint16_t);
    /**
     * This function describes the sample rate that the Pinnacle ASIC uses for
     * reporting data.
     * @rst
     * :Return:
     *     The setting configured by `setSampleRate()` or ``0`` if `Data Mode`_ is
     *     set to `PINNACLE_ANYMEAS`.
     * @endrst
     */
    uint16_t getSampleRate();
    /**
     * This function will configure the Pinnacle ASIC to detect either finger,
     * stylus, or both.
     * @rst
     * .. tip:: Consider adjusting the ADC matrix's gain to enhance
     *     performance/results using `setAdcGain()`.
     * @endrst
     * @param enableFinger `true` enables the Pinnacle ASIC's measurements to
     * detect if the touch event was caused by a finger or 5.25mm stylus.
     * `false` disables this feature. Default is `true`.
     * @param enableStylus `true` enables the Pinnacle ASIC's measurements to
     * detect if the touch event was caused by a 2mm stylus. `false` disables
     * this feature. Default is `true`.
     * @param sampleRate See the setSampleRate() as this parameter directly
     * calls that function.
     */
    void detectFingerStylus(bool enableFinger=true,
                            bool enableStylus=true,
                            uint16_t sampleRate=100);
    /**
     * @rst
     * Set calibration parameters when the Pinnacle ASIC calibrates itself.
     * This function only applies to `PINNACLE_RELATIVE` or
     * `PINNACLE_ABSOLUTE` mode, otherwise if `Data Mode`_ is set to
     * `PINNACLE_ANYMEAS`, then this function will do nothing.
     *
     * .. note:: According to the datasheet, calibration of the sensor takes about
     *     100 milliseconds. This function will block until calibration is complete
     *     (if ``run`` is ``true``). It is recommended for typical applications to
     *     leave all optional parameters in their default states.
     * @endrst
     * @param run If `true`, this function forces a calibration of the sensor.
     * If `false`, this function just writes the following parameters to the
     * Pinnacle ASIC's "CalConfig1" register. This parameter is required while
     * the rest are optional keyword parameters.
     * @param tap Enable dynamic tap compensation? Default is `true`.
     * @param trackError Enable dynamic track error compensation? Default is
     * `true`.
     * @param nerd Enable dynamic NERD compensation? Default is `true`. This
     * parameter has something to do with palm detection/compensation.
     * @param background Enable dynamic background compensation? Default is
     * `true`.
     */
    void calibrate( bool run,
                    bool tap=true,
                    bool trackError=true,
                    bool nerd=true,
                    bool background=true);
    /**
     * Manually sets the compensation matrix (array) of the 46 16-bit unsigned
     * integer values stored in the Pinnacle ASIC's memory that is used for
     * taking measurements. This matrix may not applicable in AnyMeas mode
     * (specification sheet is lacking adequate information).
     * @param matrix The array of 46 16-bit unsigned integers that will be used
     * for compensation calculations when measuring of input events.
     * @rst
     * .. seealso:: Review the hint in `getCalibrationMatrix()` from the Pinnacle
     *     ASIC's application note about deciding what values to use.
     * @endrst
     */
    void setCalibrationMatrix(int16_t*);
    /**
     * Use this function to compare a prior compensation matrix with a new
     * matrix that was either loaded manually via setCalibrationMatrix() or
     * created internally by calling calibrate() with the `run` parameter as
     * `true`.
     * @rst
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
     * @endrst
     * @param matrix A reference pointer (declared array of 46 16-bit unsigned
     * integers) for storing the compensation matrix configured by
     * setCalibrationMatrix() or created internally by calibrate() (or after a
     * "power-on-reset" condition).
     */
    void getCalibrationMatrix(int16_t*);
    /**
     * @rst
     * Sets the ADC (Analog to Digital Converter) attenuation (gain ratio) to
     * enhance performance based on the overlay type. This does not apply to
     * AnyMeas mode. However, the input value specified can be written while
     * `Data Mode`_ is set to `PINNACLE_ANYMEAS`, but there is no garauntee that
     * it will "stick" as it may be overidden by the Pinnacle ASIC
     * (specification sheet does not imply either way).
     * @endrst
     * @param sensitivity This byte specifies how sensitive the ADC component
     * is. It must be in range [0, 3]. Where `0` means most sensitive, and `3`
     * means least sensitive. A value outside this range will default to `0`.
     * @rst
     * .. tip:: The official example code from Cirque for a curved overlay uses a
     *     value of ``1``.
     * @endrst
     */
    void setAdcGain(uint8_t);
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
    void tuneEdgeSensitivity(uint8_t xAxisWideZMin=4, uint8_t yAxisWideZMin=3);
    /**
     * @rst
     * This function configures the Pinnacle ASIC for taking raw ADC
     * measurements. Be sure to set the `Data Mode`_ attribute to
     * `PINNACLE_ANYMEAS` before calling this function otherwise it will do
     * nothing.
     *
     * .. note:: The ``appertureWidth`` parameter has a inverse relationship/affect
     *     on the ``frequency`` parameter. The approximated frequencies described
     *     in this documentation are based on an aperture width of 500
     *     nanoseconds, and they will shrink as the apperture width grows or grow
     *     as the aperture width shrinks.
     * @endrst
     * @param gain Sets the sensitivity of the ADC matrix. Valid values are the
     * constants defined in @ref AnyMeasGain. Defaults to
     * @ref PINNACLE_GAIN_200.
     * @param frequency Sets the frequency of measurements made by the ADC
     * matrix. Valid values are the constants defined in
     * @ref AnyMeasFreq. Defaults @ref PINNACLE_FREQ_0.
     * @param sampleLength Sets the maximum bit length of the measurements made
     * by the ADC matrix. Valid values are `128`, `256`, or `512`. Defaults to
     * `512`.
     * @param muxControl The Pinnacle ASIC can employ different bipolar
     * junctions and/or reference capacitors. Valid values are the constants
     * defined in @ref AnyMeasMuxing. Additional combination of these
     * constants is also allowed. Defaults to @ref PINNACLE_MUX_PNP.
     * @param appertureWidth Sets the window of time (in nanoseconds) to allow
     * for the ADC to take a measurement. Valid values are multiples of 125 in
     * range [`250`, `1875`]. Erroneous values are clamped/truncated to this range.
     * @param controlPowerCount Configure the Pinnacle to perform a number of
     * measurements for each call to measureADC(). Defaults to 1. Constants
     * defined in @ref AnyMeasCtrl can be added (with `+`) to specify if
     * sleep is allowed ( @ref PINNACLE_CRTL_PWR_IDLE -- this is not default) or
     * if repetative measurements is allowed ( @ref PINNACLE_CRTL_REPEAT ) when
     * number of measurements is more than 1.
     * @rst
     * .. warning:: There is no bounds checking on the number of measurements
     *     specified here. Specifying more than 63 will trigger sleep mode after
     *     performing measuements.
     * .. hint:: Be aware that allowing the Pinnacle to enter sleep mode after
     *     taking measurements will cause a latency in consecutive calls to
     *     `measureADC()` as the Pinnacle requires about 300 milliseconds to wake up.
     * @endrst
     */
    void anyMeasModeConfig( uint8_t gain=PINNACLE_GAIN_200,
                            uint8_t frequency=PINNACLE_FREQ_0,
                            uint32_t sampleLength=512,
                            uint8_t muxControl=PINNACLE_MUX_PNP,
                            uint32_t appertureWidth=500,
                            uint8_t controlPowerCount=1);
    /**
     * @rst
     * This function instigates and returns the measurement (a signed short
     * integer) from the Pinnacle ASIC's ADC (Analog to Digital Converter)
     * matrix (only applies to AnyMeas mode). Internally, this function uses
     * the non-blocking helper functions `startMeasureAdc()` and
     * `getMeasureAdc()`, but blocks until ADC measurements are completed. Be
     * sure to set the `Data Mode`_ attribute to `PINNACLE_ANYMEAS` before
     * calling this function otherwise it will do nothing and return ``0``.
     *
     * :Return:
     *     A signed short integer. If `Data Mode`_ is not set to
     *     `PINNACLE_ANYMEAS`, then this function returns ``0`` and does nothing.
     * :4-byte Integer Format (for use as each parameter):
     *     Bits 29 and 28 represent the optional implementation of reference
     *     capacitors built into the Pinnacle ASIC. To use these capacitors, the
     *     corresponding constants (`PINNACLE_MUX_REF0` and/or
     *     `PINNACLE_MUX_REF1`) must be passed to anyMeasModeConfig() in the
     *     ``muxControl`` parameter, and their representative bits must be flagged
     *     in both the ``bitsToToggle`` and ``togglePolarity`` parameters.
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
     * .. seealso:: Review the `anymeas_mode.ino example &lt;examples.html#anymeas-mode&gt;`_ to
     *     understand how to use these
     * @endrst
     * @param bitsToToggle This 4-byte integer specifies which bits the
     * Pinnacle touch controller should toggle. A bit of `1` flags that bit
     * for toggling, and a bit of `0` signifies that the bit should remain
     * unaffected.
     * @param togglePolarity This 4-byte integer specifies which polarity the
     * specified bits (from `bitsToToggle` parameter) are toggled. A bit of
     * `1` toggles that bit positve, and a bit of `0` toggles that bit
     * negative.
     */
    int16_t measureAdc(unsigned int, unsigned int);
    /**
     * A non-blocking function to instigate ADC measurements when the
     * @ref PINNACLE_ANYMEAS mode. See parameters and table in measureAdc() as
     * this helper function's parameters are used exactly the same.
     */
    void startMeasureAdc(unsigned int, unsigned int);
    /**
     * @rst
     * A non-blocking function (meant to be used in conjunction with
     * `startMeasureAdc()`) to retreive the result of ADC measurements based on
     * parameters passed to `startMeasureAdc()`. Be sure that the `Data Mode`_
     * attribute is set to `PINNACLE_ANYMEAS` and `available()` returns ``true``
     * before calling this function otherwise it will return ``0``.
     *
     * :Return:
     *
     *     * A 16-bit integer if `available()` returns ``true`` and if `Data Mode`_ is
     *       set to `PINNACLE_ANYMEAS`.
     *     * ``0`` if `Data Mode`_ is not set to `PINNACLE_ANYMEAS` or if `available()`
     *       returns ``false``.
     * @endrst
     */
    int16_t getMeasureAdc();
    // void readRegisters(uint8_t reg, uint8_t* data, uint8_t len);
private:
    void eraWrite(uint16_t, uint8_t);
    void eraWriteBytes(uint16_t, uint8_t, uint8_t);
    void eraRead(uint16_t, uint8_t*);
    void eraReadBytes(uint16_t, uint8_t*, uint8_t);
    uint8_t _dataMode;
    uint8_t _dataReady;
    virtual void rapWrite(uint8_t, uint8_t) = 0;
    virtual void rapWriteBytes(uint8_t, uint8_t*, uint8_t) = 0;
    virtual void rapRead(uint8_t, uint8_t*) = 0;
    virtual void rapReadBytes(uint8_t, uint8_t*, uint8_t) = 0;
protected:
    /**
     * Starts the driver interface on the appropriate data bus.
     * @rst
     * :Return:
     *     - ``true`` if the Pinnacle ASIC was setup and configured properly (with data
     *       feed enabled using Relative mode).
     *     - ``false`` if the Pinnacle ASIC was unresponsive for some reason (all further
     *       operations will be nullified by setting `Data Mode`_ to ``0xFF``).
     * @endrst
     */
    bool begin();
};

/**
 * Derived class for interfacing with the Pinnacle ASIC via the SPI protocol.
 */
class PinnacleTouchSPI: public PinnacleTouch{
public:
    /**
     * Create an instance to interface with the Pinnacle ASIC over an SPI bus.
     * @rst
     * .. seealso:: `PinnacleTouch`
     * @endrst
     * @param dataReadyPin The input pin connected to the Pinnacle ASIC's
     * "Data Ready" pin.
     * @param slaveSelectPin The "slave select" pin output to the Pinnacle
     * ASIC.
     */
    PinnacleTouchSPI(uint8_t, uint8_t);
    /**
     * Starts the driver interface on the appropriate SPI bus.
     * @rst
     * :Return:
     *     - ``true`` if the Pinnacle ASIC was setup and configured properly (with data
     *       feed enabled using Relative mode).
     *     - ``false`` if the Pinnacle ASIC was unresponsive for some reason (all further
     *       operations will be nullified by setting `Data Mode`_ to ``0xFF``).
     * @endrst
     */
    bool begin();
private:
    void rapWrite(uint8_t, uint8_t);
    void rapWriteBytes(uint8_t, uint8_t*, uint8_t);
    void rapRead(uint8_t, uint8_t*);
    void rapReadBytes(uint8_t, uint8_t*, uint8_t);
    uint8_t _slaveSelect;
};

/**
 * Derived class for interfacing with the Pinnacle ASIC via the I2C protocol.
 */
class PinnacleTouchI2C: public PinnacleTouch{
public:
    /**
     * Create an instance to interface with the Pinnacle ASIC over an I2C bus.
     * @rst
     * .. seealso:: `PinnacleTouch`
     * @endrst
     * @param dataReadyPin The input pin connected to the Pinnacle ASIC's
     * "Data Ready" pin.
     * @param slaveAddress The slave I2C address of the Pinnacle ASIC.
     * Defaults to `0x2A`.
     */
    PinnacleTouchI2C(uint8_t, uint8_t slaveAddress=0x2A);
    /**
     * Starts the driver interface on the appropriate I2C bus.
     * @rst
     * :Return:
     *     - ``true`` if the Pinnacle ASIC was setup and configured properly (with data
     *       feed enabled using Relative mode).
     *     - ``false`` if the Pinnacle ASIC was unresponsive for some reason (all further
     *       operations will be nullified by setting `Data Mode`_ to ``0xFF``).
     * @endrst
     */
    bool begin();
private:
    void rapWrite(uint8_t, uint8_t);
    void rapWriteBytes(uint8_t, uint8_t*, uint8_t);
    void rapRead(uint8_t, uint8_t*);
    void rapReadBytes(uint8_t, uint8_t*, uint8_t);
    uint8_t _slaveAddress;
};
#endif

/**
 * @example examples/absolute_mode/absolute_mode.ino
 * Basic example of Absolute mode using PinnacleTouch API
 * @example examples/relative_mode/relative_mode.ino
 * Basic example of Relative mode using PinnacleTouch API
 * @example examples/anymeas_mode/anymeas_mode.ino
 * Basic example of AnyMeas mode using PinnacleTouch API
 * @example examples/usb_mouse/usb_mouse.ino
 * This basic example of PinnacleTouch API's Relative mode is uses
 * Arduino's MouseHID API to turn the Cirque GlidePoint circle trackpad
 * into a usb mouse for your computer.
 */
