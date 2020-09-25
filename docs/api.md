# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`PinnacleTouch`](#class_pinnacle_touch) | The abstract base class for driving the Pinnacle ASIC.
`class `[`PinnacleTouchI2C`](#class_pinnacle_touch_i2_c) | Derived class for interfacing with the Pinnacle ASIC via the I2C protocol.
`class `[`PinnacleTouchSPI`](#class_pinnacle_touch_s_p_i) | Derived class for interfacing with the Pinnacle ASIC via the SPI protocol.
`struct `[`AbsoluteReport`](#struct_absolute_report) | **See also**: [PinnacleTouch::reportAbsolute()](#class_pinnacle_touch_1ac9c927b443316ee5966d195f61e3ae78) This data structure used for returning data reports in absolute mode (using [PinnacleTouch::reportAbsolute()](#class_pinnacle_touch_1ac9c927b443316ee5966d195f61e3ae78) see also `Absolute Mode in examples <examples.html#absolute-mode>`_).
`struct `[`RelativeReport`](#struct_relative_report) | This data structure used for returning data reports in relative mode (using [PinnacleTouch::reportRelative()](#class_pinnacle_touch_1aa1c7ac76821105ad4d361a5ceab4dfb4) see also `Relative Mode in examples <examples.html#relative-mode>`_).

# class `PinnacleTouch` 

The abstract base class for driving the Pinnacle ASIC.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`PinnacleTouch`](#class_pinnacle_touch_1ae9d524dfa481c6484d30198a8a03a663)`(uint8_t)` | Create an instance to use as an interface with the Pinnacle ASIC touch controller.
`public void `[`feedEnabled`](#class_pinnacle_touch_1aab07a3221cc8f5dca6d169ed34af174f)`(bool)` | This function controls if the touch/button event data is reported or not. It only applies to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) or [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), then this function will do nothing. 
`public bool `[`isFeedEnabled`](#class_pinnacle_touch_1a3cd462ddc7cee12d5a82d1aea03f85bd)`()` | This function describes if the touch/button event data is to be reported or not. 
`public void `[`setDataMode`](#class_pinnacle_touch_1a4a9e5877c0eb999102af790719d8f633)`(uint8_t)` | This function controls which mode the data report is configured for. 
`public uint8_t `[`getDataMode`](#class_pinnacle_touch_1a791b6dec67581428668a902c13fd5698)`()` | This function describes which mode the data report is configured for. 
`public bool `[`isHardConfigured`](#class_pinnacle_touch_1ae381e7a9d5284688776cce4d9decad6f)`()` | This function can be used to inform applications about the factory customized hardware configuration. See note about product labeling in `Model Labeling Scheme <index.html#cc>`_. 
`public bool `[`available`](#class_pinnacle_touch_1a1dd11f5470283eb4ca19c4638b70ea7c)`()` | Use this function to detirmine if there is new data to report. Internally, this function checks if the interrupt signal on the "data
`public void `[`absoluteModeConfig`](#class_pinnacle_touch_1a685dfb4e8e377782a547c6520fb14e42)`(uint8_t zIdleCount,bool invertX,bool invertY)` | Configure settings specific to Absolute mode (reports axis positions). This function only applies to [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) or [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526), then this function does nothing. 
`public void `[`relativeModeConfig`](#class_pinnacle_touch_1abebd75f47e607943f88192157b4eef5f)`(bool rotate90,bool allTaps,bool secondaryTap,bool glideExtend,bool intellimouse)` | Configure settings specific to Relative mode (AKA Mouse mode) data reporting. This function only applies to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) or [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731), then this function does nothing. 
`public void `[`reportRelative`](#class_pinnacle_touch_1aa1c7ac76821105ad4d361a5ceab4dfb4)`(relativeReport * report)` | This function will fetch touch (and button) event data from the Pinnacle ASIC. This function only applies to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), then this function does nothing. 
`public void `[`reportAbsolute`](#class_pinnacle_touch_1ac9c927b443316ee5966d195f61e3ae78)`(absoluteReport * report)` | This function will fetch touch (and button) event data from the Pinnacle ASIC (including empty packets on ending of a touch/button event). This function only applies to PINNACLE_ABSOLUTE mode, otherwise if [the Data Mode](#group___data_mode) is set to PINNACLE_ANYMEAS, then this function does nothing. 
`public void `[`clearFlags`](#class_pinnacle_touch_1af88dbd871155814ef58da34268e0fda1)`()` | Use this function to clear the interrupt signal (digital input; active when HIGH) on the "data ready" pin (marked "DR" in the `pinout <index.html#pinout>`_ section). This function is mainly used internally when applicable, but it is left exposed if the application wants to neglect a data report when desirable.
`public void `[`allowSleep`](#class_pinnacle_touch_1a7ed02572b71c80df82d4629f24de5dc4)`(bool)` | This will specify if the Pinnacle ASIC is allowed to sleep after about 5 seconds of idle activity (no input event). 
`public bool `[`isAllowSleep`](#class_pinnacle_touch_1a741205ed913991e5357be8497a8e9589)`()` | This function describes if the Pinnacle ASIC is configured to enter sleep mode. This does not apply to AnyMeas mode. 
`public void `[`shutdown`](#class_pinnacle_touch_1ab69f2ae23afbcb49ee1ce3bdf3f26a0b)`(bool)` | This function controls power state of the Pinnacle ASIC that drives the touchpad. 
`public bool `[`isShutdown`](#class_pinnacle_touch_1a8b4f25247afc291e69707d5bda26a163)`()` | This function describes if the Pinnacle ASIC is in a power down mode or not. 
`public void `[`setSampleRate`](#class_pinnacle_touch_1a1b4ffa26467468d9a5a46aab7c55a304)`(uint16_t)` | This function controls how many samples (of data) per second are taken. This function only applies to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) or [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), then this function will do nothing. 
`public uint16_t `[`getSampleRate`](#class_pinnacle_touch_1aa79301cfcc9ea8e8f11aed35616810dc)`()` | This function describes the sample rate that the Pinnacle ASIC uses for reporting data. 
`public void `[`detectFingerStylus`](#class_pinnacle_touch_1a524b5c6e7ae6a780f245b2518ef54a0f)`(bool enableFinger,bool enableStylus,uint16_t sampleRate)` | This function will configure the Pinnacle ASIC to detect either finger, stylus, or both. 
`public void `[`calibrate`](#class_pinnacle_touch_1a90531beb0f302ee43276ba8e3003c282)`(bool run,bool tap,bool trackError,bool nerd,bool background)` | Set calibration parameters when the Pinnacle ASIC calibrates itself. This function only applies to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) or [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), then this function will do nothing. 
`public void `[`setCalibrationMatrix`](#class_pinnacle_touch_1a7020121a555f4ae3aba358b04e779940)`(int16_t *)` | Manually sets the compensation matrix (array) of the 46 16-bit unsigned integer values stored in the Pinnacle ASIC's memory that is used for taking measurements. This matrix may not applicable in AnyMeas mode (specification sheet is lacking adequate information). 
`public void `[`getCalibrationMatrix`](#class_pinnacle_touch_1a024f7358efb08a08307bf4f5fbacf2d8)`(int16_t *)` | Use this function to compare a prior compensation matrix with a new matrix that was either loaded manually via [setCalibrationMatrix()](#class_pinnacle_touch_1a7020121a555f4ae3aba358b04e779940) or created internally by calling [calibrate()](#class_pinnacle_touch_1a90531beb0f302ee43276ba8e3003c282) with the `run` parameter as `true`. 
`public void `[`setAdcGain`](#class_pinnacle_touch_1ad3fbd5924b5d39ebbea0da406caca8bf)`(uint8_t)` | Sets the ADC (Analog to Digital Converter) attenuation (gain ratio) to enhance performance based on the overlay type. This does not apply to AnyMeas mode. However, the input value specified can be written while [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), but there is no garauntee that it will "stick" as it may be overidden by the Pinnacle ASIC (specification sheet does not imply either way). 
`public void `[`tuneEdgeSensitivity`](#class_pinnacle_touch_1ad58f66ccf236679e95162645e49e8c3a)`(uint8_t xAxisWideZMin,uint8_t yAxisWideZMin)` | According to the comments in the official example code from Cirque, "Changes thresholds to improve detection of fingers." This function was ported from Cirque's example code and doesn't have corresponding documentation. Thus, the defaults for this function's parameters use the same values found in the official example. I'm unaware of any documented memory map for the Pinnacle ASIC as this function directly alters values in the Pinnacle ASIC's memory.USE THIS AT YOUR OWN RISK!
`public void `[`anyMeasModeConfig`](#class_pinnacle_touch_1a3380432fb79add7a6d44ccd817084f28)`(uint8_t gain,uint8_t frequency,uint32_t sampleLength,uint8_t muxControl,uint32_t appertureWidth,uint8_t controlPowerCount)` | This function configures the Pinnacle ASIC for taking raw ADC measurements. Be sure to set the [the Data Mode](#group___data_mode) attribute to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) before calling this function otherwise it will do nothing. 
`public int16_t `[`measureAdc`](#class_pinnacle_touch_1af28cb9063313ff38b322641c8a26921c)`(unsigned int,unsigned int)` | This function instigates and returns the measurement (a signed short integer) from the Pinnacle ASIC's ADC (Analog to Digital Converter) matrix (only applies to AnyMeas mode). Internally, this function uses the non-blocking helper functions [startMeasureAdc()](#class_pinnacle_touch_1aec3e8e56aba864d8ad006c4a339a3376) and [getMeasureAdc()](#class_pinnacle_touch_1af32f069cbd7414133149102ffaf8f208), but blocks until ADC measurements are completed. Be sure to set the [the Data Mode](#group___data_mode) attribute to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) before calling this function otherwise it will do nothing and return `0`. 
`public void `[`startMeasureAdc`](#class_pinnacle_touch_1aec3e8e56aba864d8ad006c4a339a3376)`(unsigned int,unsigned int)` | A non-blocking function to instigate ADC measurements when the [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) mode. See parameters and table in [measureAdc()](#class_pinnacle_touch_1af28cb9063313ff38b322641c8a26921c) as this helper function's parameters are used exactly the same.
`public int16_t `[`getMeasureAdc`](#class_pinnacle_touch_1af32f069cbd7414133149102ffaf8f208)`()` | A non-blocking function (meant to be used in conjunction with [startMeasureAdc()](#class_pinnacle_touch_1aec3e8e56aba864d8ad006c4a339a3376)) to retreive the result of ADC measurements based on parameters passed to [startMeasureAdc()](#class_pinnacle_touch_1aec3e8e56aba864d8ad006c4a339a3376). Be sure that the [the Data Mode](#group___data_mode) attribute is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) and [available()](#class_pinnacle_touch_1a1dd11f5470283eb4ca19c4638b70ea7c) returns `true` before calling this function otherwise it will return `0`. 
`protected bool `[`begin`](#class_pinnacle_touch_1a9f24a3341818325fb74207b27ea2e83b)`()` | Starts the driver interface on the appropriate data bus. 

## Members

#### `public  `[`PinnacleTouch`](#class_pinnacle_touch_1ae9d524dfa481c6484d30198a8a03a663)`(uint8_t)` 

Create an instance to use as an interface with the Pinnacle ASIC touch controller.

#### Parameters
* `dataReadyPin` The input pin connected to the Pinnacle ASIC's "Data Ready" pin.

#### `public void `[`feedEnabled`](#class_pinnacle_touch_1aab07a3221cc8f5dca6d169ed34af174f)`(bool)` 

This function controls if the touch/button event data is reported or not. It only applies to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) or [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), then this function will do nothing. 
#### Parameters
* `isEnabled` `true` enables data reporting; `false` disables data reporting.

#### `public bool `[`isFeedEnabled`](#class_pinnacle_touch_1a3cd462ddc7cee12d5a82d1aea03f85bd)`()` 

This function describes if the touch/button event data is to be reported or not. 
#### Returns
The setting configured by [feedEnabled()](#class_pinnacle_touch_1aab07a3221cc8f5dca6d169ed34af174f) or `false` if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7).

#### `public void `[`setDataMode`](#class_pinnacle_touch_1a4a9e5877c0eb999102af790719d8f633)`(uint8_t)` 

This function controls which mode the data report is configured for. 
#### Parameters
* `mode` Valid input values are [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) for relative/mouse mode, [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731) for absolute positioning mode, or [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) (referred to as "AnyMeas" in specification sheet) mode for reading ADC values. Invalid input values have no affect.

#### `public uint8_t `[`getDataMode`](#class_pinnacle_touch_1a791b6dec67581428668a902c13fd5698)`()` 

This function describes which mode the data report is configured for. 
#### Returns

* `0` (AKA [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526)) for Relative mode (AKA mouse mode)

* `1` (AKA [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7)) for AnyMeas mode (raw ADC measurements)

* `2` (AKA [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731)) for Absolute mode (X & Y axis positions)

* `255` if [begin()](#class_pinnacle_touch_1a9f24a3341818325fb74207b27ea2e83b) returns `false` (failed to initialize the trackpad) 

ImportantWhen switching from [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) or [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731) all configurations are reset, and must be re-configured by using [absoluteModeConfig()](#class_pinnacle_touch_1a685dfb4e8e377782a547c6520fb14e42) or [relativeModeConfig()](#class_pinnacle_touch_1abebd75f47e607943f88192157b4eef5f).

#### `public bool `[`isHardConfigured`](#class_pinnacle_touch_1ae381e7a9d5284688776cce4d9decad6f)`()` 

This function can be used to inform applications about the factory customized hardware configuration. See note about product labeling in `Model Labeling Scheme <index.html#cc>`_. 
#### Returns
`true` if a 470K ohm resistor is populated at the junction labeled "R4"; `false` if no resistor is populated at the "R4" junction. This function will also return `false` if [begin()](#class_pinnacle_touch_1a9f24a3341818325fb74207b27ea2e83b) failed to initialize the trackpad.

#### `public bool `[`available`](#class_pinnacle_touch_1a1dd11f5470283eb4ca19c4638b70ea7c)`()` 

Use this function to detirmine if there is new data to report. Internally, this function checks if the interrupt signal on the "data
ready" pin (labeled "DR" in the `pinout <index.html#pinout>`_ section) is active. Data (new or antiquated) can be retreived using [reportRelative()](#class_pinnacle_touch_1aa1c7ac76821105ad4d361a5ceab4dfb4) or [reportAbsolute()](#class_pinnacle_touch_1ac9c927b443316ee5966d195f61e3ae78) depending on what [the Data Mode](#group___data_mode) is set to. 
#### Returns
`true` if there is new data to report; `false` if there is no new data to report.

#### `public void `[`absoluteModeConfig`](#class_pinnacle_touch_1a685dfb4e8e377782a547c6520fb14e42)`(uint8_t zIdleCount,bool invertX,bool invertY)` 

Configure settings specific to Absolute mode (reports axis positions). This function only applies to [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) or [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526), then this function does nothing. 
#### Parameters
* `zIdleCount` Specifies the number of empty packets (x-axis, y-axis, and z-axis are `0`) reported (every 10 milliseconds) when there is no touch detected. Defaults to 30. This number is clamped to range [0, 255]. 

* `invertX` Specifies if the x-axis data is to be inverted before reporting it. Default is `false`. 

* `invertY` Specifies if the y-axis data is to be inverted before reporting it. Default is `false`.

#### `public void `[`relativeModeConfig`](#class_pinnacle_touch_1abebd75f47e607943f88192157b4eef5f)`(bool rotate90,bool allTaps,bool secondaryTap,bool glideExtend,bool intellimouse)` 

Configure settings specific to Relative mode (AKA Mouse mode) data reporting. This function only applies to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) or [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731), then this function does nothing. 
#### Parameters
* `rotate90` Specifies if the axis data is altered for 90 degree rotation before reporting it (essentially swaps the axis data). Default is `false`. 

* `allTaps` Specifies if all taps should be reported (`true`) or not (`false`). Default is `true`. This affects `secondaryTap` option as well. 

* `secondaryTap` Specifies if tapping in the top-left corner (depending on orientation) triggers the secondary button data. Defaults to `true`. This feature is always disabled if [isHardConfigured()](#class_pinnacle_touch_1ae381e7a9d5284688776cce4d9decad6f) is `true`. 

* `glideExtend` A patended feature that allows the user to glide their finger off the edge of the sensor and continue gesture with the touch event. Default is `true`. This feature is always disabled if [isHardConfigured()](#class_pinnacle_touch_1ae381e7a9d5284688776cce4d9decad6f) is `true`. 

* `intellimouse` Specifies if the data reported includes a byte about scroll data. Default is `false`. This feature is always disabled if [isHardConfigured()](#class_pinnacle_touch_1ae381e7a9d5284688776cce4d9decad6f) is `true`.

#### `public void `[`reportRelative`](#class_pinnacle_touch_1aa1c7ac76821105ad4d361a5ceab4dfb4)`(relativeReport * report)` 

This function will fetch touch (and button) event data from the Pinnacle ASIC. This function only applies to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), then this function does nothing. 
#### Parameters
* `report` A reference pointer (declared variable of datatype [RelativeReport](#struct_relative_report)) for storing the data that describes the touch (and button) event.

#### `public void `[`reportAbsolute`](#class_pinnacle_touch_1ac9c927b443316ee5966d195f61e3ae78)`(absoluteReport * report)` 

This function will fetch touch (and button) event data from the Pinnacle ASIC (including empty packets on ending of a touch/button event). This function only applies to PINNACLE_ABSOLUTE mode, otherwise if [the Data Mode](#group___data_mode) is set to PINNACLE_ANYMEAS, then this function does nothing. 
#### Parameters
* `report` A reference pointer (declared variable of datatype [AbsoluteReport](#struct_absolute_report)) for storing the data that describes the touch (and button) event.

#### `public void `[`clearFlags`](#class_pinnacle_touch_1af88dbd871155814ef58da34268e0fda1)`()` 

Use this function to clear the interrupt signal (digital input; active when HIGH) on the "data ready" pin (marked "DR" in the `pinout <index.html#pinout>`_ section). This function is mainly used internally when applicable, but it is left exposed if the application wants to neglect a data report when desirable.

#### `public void `[`allowSleep`](#class_pinnacle_touch_1a7ed02572b71c80df82d4629f24de5dc4)`(bool)` 

This will specify if the Pinnacle ASIC is allowed to sleep after about 5 seconds of idle activity (no input event). 
#### Parameters
* `isEnabled` `true` if you want the Pinnacle ASIC to enter sleep (low power) mode after about 5 seconds of inactivity (does not apply to AnyMeas mode). `false` if you don't want the Pinnacle ASIC to enter sleep mode. 

While the touch controller is in sleep mode, if a touch event or button press is detected, the Pinnacle ASIC will take about 300 milliseconds to wake up (does not include handling the touch event or button press data). Remember that releasing a held button is also considered an input event.

#### `public bool `[`isAllowSleep`](#class_pinnacle_touch_1a741205ed913991e5357be8497a8e9589)`()` 

This function describes if the Pinnacle ASIC is configured to enter sleep mode. This does not apply to AnyMeas mode. 
#### Returns
The setting configured by [allowSleep()](#class_pinnacle_touch_1a7ed02572b71c80df82d4629f24de5dc4) or `false` if [begin()](#class_pinnacle_touch_1a9f24a3341818325fb74207b27ea2e83b) failed to initialize the trackpad.

#### `public void `[`shutdown`](#class_pinnacle_touch_1ab69f2ae23afbcb49ee1ce3bdf3f26a0b)`(bool)` 

This function controls power state of the Pinnacle ASIC that drives the touchpad. 
#### Parameters
* `isOff` `true` means power down (AKA standby mode), and `false` means power up (Active, Idle, or Sleep mode). 

The ASIC will take about 300 milliseconds to complete the transition from powered down mode to active mode. No touch events or button presses will be monitored while powered down.

#### `public bool `[`isShutdown`](#class_pinnacle_touch_1a8b4f25247afc291e69707d5bda26a163)`()` 

This function describes if the Pinnacle ASIC is in a power down mode or not. 
#### Returns
The setting configured by [shutdown()](#class_pinnacle_touch_1ab69f2ae23afbcb49ee1ce3bdf3f26a0b)

#### `public void `[`setSampleRate`](#class_pinnacle_touch_1a1b4ffa26467468d9a5a46aab7c55a304)`(uint16_t)` 

This function controls how many samples (of data) per second are taken. This function only applies to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) or [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), then this function will do nothing. 
#### Parameters
* `value` Valid input values are `100`, `80`, `60`, `40`, `20`, `10`. Any other input values automatically set the sample rate to 100 sps (samples per second). Optionally, `200` and `300` sps can be specified, but using these optional values automatically disables palm (referred to as "NERD" in the specification sheet) and noise compensations. These higher values are meant for using a stylus with a 2mm diameter tip, while the values less than 200 are meant for a finger or stylus with a 5.25mm diameter tip.

#### `public uint16_t `[`getSampleRate`](#class_pinnacle_touch_1aa79301cfcc9ea8e8f11aed35616810dc)`()` 

This function describes the sample rate that the Pinnacle ASIC uses for reporting data. 
#### Returns
The setting configured by [setSampleRate()](#class_pinnacle_touch_1a1b4ffa26467468d9a5a46aab7c55a304) or 0 if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7).

#### `public void `[`detectFingerStylus`](#class_pinnacle_touch_1a524b5c6e7ae6a780f245b2518ef54a0f)`(bool enableFinger,bool enableStylus,uint16_t sampleRate)` 

This function will configure the Pinnacle ASIC to detect either finger, stylus, or both. 
#### Parameters
* `enableFinger` `true` enables the Pinnacle ASIC's measurements to detect if the touch event was caused by a finger or 5.25mm stylus. `false` disables this feature. Default is `true`. 

* `enableStylus` `true` enables the Pinnacle ASIC's measurements to detect if the touch event was caused by a 2mm stylus. `false` disables this feature. Default is `true`. 

* `sampleRate` See the [setSampleRate()](#class_pinnacle_touch_1a1b4ffa26467468d9a5a46aab7c55a304) as this parameter directly calls that function. 

TipConsider adjusting the ADC matrix's gain to enhance performance/results using [setAdcGain()](#class_pinnacle_touch_1ad3fbd5924b5d39ebbea0da406caca8bf).

#### `public void `[`calibrate`](#class_pinnacle_touch_1a90531beb0f302ee43276ba8e3003c282)`(bool run,bool tap,bool trackError,bool nerd,bool background)` 

Set calibration parameters when the Pinnacle ASIC calibrates itself. This function only applies to [PINNACLE_RELATIVE](#group___data_mode_1gad67f066348cfcd556403baccb092e526) or [PINNACLE_ABSOLUTE](#group___data_mode_1gac2bd445639a9682a76c4a47c9c018731) mode, otherwise if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), then this function will do nothing. 
#### Parameters
* `run` If `true`, this function forces a calibration of the sensor. If `false`, this function just writes the following parameters to the Pinnacle ASIC's "CalConfig1" register. This parameter is required while the rest are optional keyword parameters. 

* `tap` Enable dynamic tap compensation? Default is `true`. 

* `trackError` Enable dynamic track error compensation? Default is `true`. 

* `nerd` Enable dynamic NERD compensation? Default is `true`. This parameter has something to do with palm detection/compensation. 

* `background` Enable dynamic background compensation? Default is `true`. 

According to the datasheet, calibration of the sensor takes about 100 milliseconds. This function will block until calibration is complete (if `run` is `true`). It is recommended for typical applications to leave all optional parameters in their default states.

#### `public void `[`setCalibrationMatrix`](#class_pinnacle_touch_1a7020121a555f4ae3aba358b04e779940)`(int16_t *)` 

Manually sets the compensation matrix (array) of the 46 16-bit unsigned integer values stored in the Pinnacle ASIC's memory that is used for taking measurements. This matrix may not applicable in AnyMeas mode (specification sheet is lacking adequate information). 
#### Parameters
* `matrix` The array of 46 16-bit unsigned integers that will be used for compensation calculations when measuring of input events. See note below from the Pinnacle ASIC's application note in [getCalibrationMatrix()](#class_pinnacle_touch_1a024f7358efb08a08307bf4f5fbacf2d8) about deciding what values to use.

#### `public void `[`getCalibrationMatrix`](#class_pinnacle_touch_1a024f7358efb08a08307bf4f5fbacf2d8)`(int16_t *)` 

Use this function to compare a prior compensation matrix with a new matrix that was either loaded manually via [setCalibrationMatrix()](#class_pinnacle_touch_1a7020121a555f4ae3aba358b04e779940) or created internally by calling [calibrate()](#class_pinnacle_touch_1a90531beb0f302ee43276ba8e3003c282) with the `run` parameter as `true`. 
#### Parameters
* `matrix` A reference pointer (declared array of 46 16-bit unsigned integers) for storing the compensation matrix configured by [setCalibrationMatrix()](#class_pinnacle_touch_1a7020121a555f4ae3aba358b04e779940) or created internally by [calibrate()](#class_pinnacle_touch_1a90531beb0f302ee43276ba8e3003c282) (or after a "power-on-reset" condition). 

A note from Cirque's Application Note on Comparing matrices:If any 16-bit values are above 20K (absolute), it generally indicates a problem with the sensor. If no values exceed 20K, proceed with the data comparison. Compare each 16-bit value in one matrix to the corresponding 16-bit value in the other matrix. If the difference between the two values is greater than 500 (absolute), it indicates a change in the environment. Either an object was on the sensor during calibration, or the surrounding conditions (temperature, humidity, or noise level) have changed. One strategy is to force another calibration and compare again, if the values continue to differ by 500, determine whether to use the new data or a previous set of stored data. Another strategy is to average any two values that differ by more than 500 and write this new matrix, with the average values, back into Pinnacle ASIC.

#### `public void `[`setAdcGain`](#class_pinnacle_touch_1ad3fbd5924b5d39ebbea0da406caca8bf)`(uint8_t)` 

Sets the ADC (Analog to Digital Converter) attenuation (gain ratio) to enhance performance based on the overlay type. This does not apply to AnyMeas mode. However, the input value specified can be written while [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), but there is no garauntee that it will "stick" as it may be overidden by the Pinnacle ASIC (specification sheet does not imply either way). 
#### Parameters
* `sensitivity` This byte specifies how sensitive the ADC component is. It must be in range [0, 3]. Where `0` means most sensitive, and `3` means least sensitive. A value outside this range will default to `0`. 

TipThe official example code from Cirque for a curved overlay uses a value of `1`.

#### `public void `[`tuneEdgeSensitivity`](#class_pinnacle_touch_1ad58f66ccf236679e95162645e49e8c3a)`(uint8_t xAxisWideZMin,uint8_t yAxisWideZMin)` 

According to the comments in the official example code from Cirque, "Changes thresholds to improve detection of fingers." This function was ported from Cirque's example code and doesn't have corresponding documentation. Thus, the defaults for this function's parameters use the same values found in the official example. I'm unaware of any documented memory map for the Pinnacle ASIC as this function directly alters values in the Pinnacle ASIC's memory.USE THIS AT YOUR OWN RISK!

#### `public void `[`anyMeasModeConfig`](#class_pinnacle_touch_1a3380432fb79add7a6d44ccd817084f28)`(uint8_t gain,uint8_t frequency,uint32_t sampleLength,uint8_t muxControl,uint32_t appertureWidth,uint8_t controlPowerCount)` 

This function configures the Pinnacle ASIC for taking raw ADC measurements. Be sure to set the [the Data Mode](#group___data_mode) attribute to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) before calling this function otherwise it will do nothing. 
#### Parameters
* `gain` Sets the sensitivity of the ADC matrix. Valid values are the constants defined in [AnyMeas Gain](#group___any_meas_gain). Defaults to [PINNACLE_GAIN_200](#group___any_meas_gain_1gac493d7c70df8e878d7bc22bb8d48bf98). 

* `frequency` Sets the frequency of measurements made by the ADC matrix. Valid values are the constants defined in [AnyMeas Frequncy](#group___any_meas_freq). Defaults [PINNACLE_FREQ_0](#group___any_meas_freq_1ga1dd2f344c8b0c897b6f66d595ef4a87b). 

* `sampleLength` Sets the maximum bit length of the measurements made by the ADC matrix. Valid values are `128`, `256`, or `512`. Defaults to `512`. 

* `muxControl` The Pinnacle ASIC can employ different bipolar junctions and/or reference capacitors. Valid values are the constants defined in [AnyMeas Matrix Muxing](#group___any_meas_muxing). Additional combination of these constants is also allowed. Defaults to [PINNACLE_MUX_PNP](#group___any_meas_muxing_1ga0df815e210e4e208e882640ccd98f334). 

* `appertureWidth` Sets the window of time (in nanoseconds) to allow for the ADC to take a measurement. Valid values are multiples of 125 in range [`250`, `1875`]. Erroneous values are clamped/truncated to this range. 

* `controlPowerCount` Configure the Pinnacle to perform a number of measurements for each call to measureADC(). Defaults to 1. Constants defined in [AnyMeas Control](#group___any_meas_ctrl) can be added (with `+`) to specify if sleep is allowed ( [PINNACLE_CRTL_PWR_IDLE](#group___any_meas_ctrl_1ga099c9ad4fff3f956fb4a667b4b1405d3) this is not default) or if repetative measurements is allowed ( [PINNACLE_CRTL_REPEAT](#group___any_meas_ctrl_1ga10ca9d82959f23dfcda0246f3c2ecbb3) ) when number of measurements is more than 1. 

The `appertureWidth` parameter has a inverse relationship/affect on the `frequency` parameter. The approximated frequencies described in this documentation are based on an aperture width of 500 nanoseconds, and they will shrink as the apperture width grows or grow as the aperture width shrinks. 

There is no bounds checking on the number of measurements specified here. Specifying more than 63 will trigger sleep mode after performing measuements. 

HintBe aware that allowing the Pinnacle to enter sleep mode after taking measurements will cause a latency in consecutive calls to measureADC() as the Pinnacle requires about 300 milliseconds to wake up.

#### `public int16_t `[`measureAdc`](#class_pinnacle_touch_1af28cb9063313ff38b322641c8a26921c)`(unsigned int,unsigned int)` 

This function instigates and returns the measurement (a signed short integer) from the Pinnacle ASIC's ADC (Analog to Digital Converter) matrix (only applies to AnyMeas mode). Internally, this function uses the non-blocking helper functions [startMeasureAdc()](#class_pinnacle_touch_1aec3e8e56aba864d8ad006c4a339a3376) and [getMeasureAdc()](#class_pinnacle_touch_1af32f069cbd7414133149102ffaf8f208), but blocks until ADC measurements are completed. Be sure to set the [the Data Mode](#group___data_mode) attribute to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) before calling this function otherwise it will do nothing and return `0`. 
#### Parameters
* `bitsToToggle` This 4-byte integer specifies which bits the Pinnacle touch controller should toggle. A bit of `1` flags that bit for toggling, and a bit of `0` signifies that the bit should remain unaffected. 

* `togglePolarity` This 4-byte integer specifies which polarity the specified bits (from `bitsToToggle` parameter) are toggled. A bit of `1` toggles that bit positve, and a bit of `0` toggles that bit negative. 

#### Returns
A signed short integer. If [the Data Mode](#group___data_mode) is not set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7), then this function returns `0` and does nothing.

4-byte Integer Format

byte 3 (MSByte)
---------
bit position  |31  |30  |29  |28  |27  |26  |25  |24
representation  |N/A  |N/A  |Ref1  |Ref0  |Y11  |Y10  |Y9  |Y8

byte 2
---------
bit position  |23  |22  |21  |20  |19  |18  |17  |16
representation  |Y7  |Y6  |Y5  |Y4  |Y3  |Y2  |Y1  |Y0

byte 1
---------
bit position  |15  |14  |13  |12  |11  |10  |9  |8
representation  |X15  |X14  |X13  |X12  |X11  |X10  |X9  |X8

byte 0 (LSByte)
---------
bit position  |7  |6  |5  |4  |3  |2  |1  |0
representation  |X7  |X6  |X5  |X4  |X3  |X2  |X1  |X0
See `AnyMeas mode example <examples.html#anymeas-mode>`_ to understand how to use these 4-byte integer polynomials. Bits 29 and 28 represent the optional implementation of reference capacitors built into the Pinnacle ASIC. To use these capacitors, the corresponding constants (`PINNACLE_MUX_REF0` and/or `PINNACLE_MUX_REF1`) must be passed to [anyMeasModeConfig()](#class_pinnacle_touch_1a3380432fb79add7a6d44ccd817084f28) in the `muxControl` parameter, and their representative bits must be flagged in both the `bitsToToggle` & `togglePolarity` parameters.

#### `public void `[`startMeasureAdc`](#class_pinnacle_touch_1aec3e8e56aba864d8ad006c4a339a3376)`(unsigned int,unsigned int)` 

A non-blocking function to instigate ADC measurements when the [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) mode. See parameters and table in [measureAdc()](#class_pinnacle_touch_1af28cb9063313ff38b322641c8a26921c) as this helper function's parameters are used exactly the same.

#### `public int16_t `[`getMeasureAdc`](#class_pinnacle_touch_1af32f069cbd7414133149102ffaf8f208)`()` 

A non-blocking function (meant to be used in conjunction with [startMeasureAdc()](#class_pinnacle_touch_1aec3e8e56aba864d8ad006c4a339a3376)) to retreive the result of ADC measurements based on parameters passed to [startMeasureAdc()](#class_pinnacle_touch_1aec3e8e56aba864d8ad006c4a339a3376). Be sure that the [the Data Mode](#group___data_mode) attribute is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) and [available()](#class_pinnacle_touch_1a1dd11f5470283eb4ca19c4638b70ea7c) returns `true` before calling this function otherwise it will return `0`. 
#### Returns

* A 16-bit integer if [available()](#class_pinnacle_touch_1a1dd11f5470283eb4ca19c4638b70ea7c) returns `true` and if [the Data Mode](#group___data_mode) is set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7).

* `0` if [the Data Mode](#group___data_mode) is not set to [PINNACLE_ANYMEAS](#group___data_mode_1ga8782c10eec1bbc1af8fdcc1ca7a658d7) or if [available()](#class_pinnacle_touch_1a1dd11f5470283eb4ca19c4638b70ea7c) returns `false`.

#### `protected bool `[`begin`](#class_pinnacle_touch_1a9f24a3341818325fb74207b27ea2e83b)`()` 

Starts the driver interface on the appropriate data bus. 
#### Returns

* `true` if the Pinnacle ASIC was setup and configured properly (with data feed enabled using Relative mode).

* `false` if the Pinnacle ASIC was unresponsive for some reason (all further operations will be nullified by setting [the Data Mode](#group___data_mode) to `0xFF`).

# class `PinnacleTouchI2C` 

```
class PinnacleTouchI2C
  : public PinnacleTouch
```  

Derived class for interfacing with the Pinnacle ASIC via the I2C protocol.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`PinnacleTouchI2C`](#class_pinnacle_touch_i2_c_1a461c974a2d6a6fedd4607ea141043838)`(uint8_t,uint8_t slaveAddress)` | Create an instance to interface with the Pinnacle ASIC over an I2C bus. 
`public bool `[`begin`](#class_pinnacle_touch_i2_c_1ade5e6c674b7684339e5b6e3505bbf708)`()` | Starts the driver interface on the appropriate I2C bus. 

## Members

#### `public  `[`PinnacleTouchI2C`](#class_pinnacle_touch_i2_c_1a461c974a2d6a6fedd4607ea141043838)`(uint8_t,uint8_t slaveAddress)` 

Create an instance to interface with the Pinnacle ASIC over an I2C bus. 
#### Parameters
* `dataReadyPin` The input pin connected to the Pinnacle ASIC's "Data Ready" pin. 

* `slaveAddress` The slave I2C address of the Pinnacle ASIC. Defaults to `0x2A`. 

**See also**: [PinnacleTouch](#class_pinnacle_touch)

#### `public bool `[`begin`](#class_pinnacle_touch_i2_c_1ade5e6c674b7684339e5b6e3505bbf708)`()` 

Starts the driver interface on the appropriate I2C bus. 
#### Returns

* `true` if the Pinnacle ASIC was setup and configured properly (with data feed enabled using Relative mode).

* `false` if the Pinnacle ASIC was unresponsive for some reason (all further operations will be nullified by setting [the Data Mode](#group___data_mode) to `0xFF`).

# class `PinnacleTouchSPI` 

```
class PinnacleTouchSPI
  : public PinnacleTouch
```  

Derived class for interfacing with the Pinnacle ASIC via the SPI protocol.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`PinnacleTouchSPI`](#class_pinnacle_touch_s_p_i_1af64537697a3a6697ed13195575f78c6f)`(uint8_t,uint8_t)` | Create an instance to interface with the Pinnacle ASIC over an SPI bus. 
`public bool `[`begin`](#class_pinnacle_touch_s_p_i_1ae39ae51c77604381d43a237bd70f5136)`()` | Starts the driver interface on the appropriate SPI bus. 

## Members

#### `public  `[`PinnacleTouchSPI`](#class_pinnacle_touch_s_p_i_1af64537697a3a6697ed13195575f78c6f)`(uint8_t,uint8_t)` 

Create an instance to interface with the Pinnacle ASIC over an SPI bus. 
#### Parameters
* `dataReadyPin` The input pin connected to the Pinnacle ASIC's "Data Ready" pin. 

* `slaveSelectPin` The "slave select" pin output to the Pinnacle ASIC. 

**See also**: [PinnacleTouch](#class_pinnacle_touch)

#### `public bool `[`begin`](#class_pinnacle_touch_s_p_i_1ae39ae51c77604381d43a237bd70f5136)`()` 

Starts the driver interface on the appropriate SPI bus. 
#### Returns

* `true` if the Pinnacle ASIC was setup and configured properly (with data feed enabled using Relative mode).

* `false` if the Pinnacle ASIC was unresponsive for some reason (all further operations will be nullified by setting [the Data Mode](#group___data_mode) to `0xFF`).

# struct `AbsoluteReport` 

**See also**: [PinnacleTouch::reportAbsolute()](#class_pinnacle_touch_1ac9c927b443316ee5966d195f61e3ae78) This data structure used for returning data reports in absolute mode (using [PinnacleTouch::reportAbsolute()](#class_pinnacle_touch_1ac9c927b443316ee5966d195f61e3ae78) see also `Absolute Mode in examples <examples.html#absolute-mode>`_).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`buttons`](#struct_absolute_report_1a6f2b7dbf18dcaa5bff8ce2f1fe716b85) | This will always be in range [0, 7].
`public uint16_t `[`x`](#struct_absolute_report_1a4738ca2aec11446e855429992ba9bbed) | This will always be in range 128 <= `x` <= 1920.
`public uint16_t `[`y`](#struct_absolute_report_1ad5921f23333d448e18ab0ae907b3a173) | This will always be in range 64 <= `y` <= 1472.
`public uint8_t `[`z`](#struct_absolute_report_1a008a625a616d98acd3b6c38f75e29242) | This will always be in range 0 <= `z` <= 65535. The maximum value will depend on senitivity.

## Members

#### `public uint8_t `[`buttons`](#struct_absolute_report_1a6f2b7dbf18dcaa5bff8ce2f1fe716b85) 

This will always be in range [0, 7].

The returned button data is a byte in which each bit represents a button. The bit to button order is as follows:

* 0. [LSB] Button 1

* 1. Button 2

* 2. Button 3

#### `public uint16_t `[`x`](#struct_absolute_report_1a4738ca2aec11446e855429992ba9bbed) 

This will always be in range 128 <= `x` <= 1920.

#### `public uint16_t `[`y`](#struct_absolute_report_1ad5921f23333d448e18ab0ae907b3a173) 

This will always be in range 64 <= `y` <= 1472.

#### `public uint8_t `[`z`](#struct_absolute_report_1a008a625a616d98acd3b6c38f75e29242) 

This will always be in range 0 <= `z` <= 65535. The maximum value will depend on senitivity.

# struct `RelativeReport` 

This data structure used for returning data reports in relative mode (using [PinnacleTouch::reportRelative()](#class_pinnacle_touch_1aa1c7ac76821105ad4d361a5ceab4dfb4) see also `Relative Mode in examples <examples.html#relative-mode>`_).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public uint8_t `[`buttons`](#struct_relative_report_1a91e12cca7927ac21d0d9f183359db4dd) | This will always be in range [0, 7].
`public int8_t `[`x`](#struct_relative_report_1afea516f255dade5afc23b45ba7d29eaf) | This will always be in range -128 <= `x` <= 127.
`public int8_t `[`y`](#struct_relative_report_1a336a8acf2a0391127ed1c426e1118b83) | This will always be in range -128 <= `y` <= 127.
`public int8_t `[`scroll`](#struct_relative_report_1a89f89a8919eaac29b04cbec6d9e6ddc2) | This will always be in range -128 <= `scroll` <= 127.

## Members

#### `public uint8_t `[`buttons`](#struct_relative_report_1a91e12cca7927ac21d0d9f183359db4dd) 

This will always be in range [0, 7].

The returned button data is a byte in which each bit represents if a button is pressed. The bit to button order is as follows:

* 0. [LSB] Button 1 (thought of as Left mouse button). If `allTaps` parameter is passed as `true` when calling [PinnacleTouch::relativeModeConfig()](#class_pinnacle_touch_1abebd75f47e607943f88192157b4eef5f), a single tap will be reflected here.

* 1. Button 2 (thought of as Right mouse button). If `allTaps` and `secondaryTap` parameters are passed as `true` when calling [PinnacleTouch::relativeModeConfig()](#class_pinnacle_touch_1abebd75f47e607943f88192157b4eef5f), a single tap in the perspective top-left-most corner will be reflected here (secondary taps are constantly disabled if isHardConfigured() returns `true`). Note that the top-left-most corner can be perspectively moved if `rotate90` parameter is passed as `true` when calling [PinnacleTouch::relativeModeConfig()](#class_pinnacle_touch_1abebd75f47e607943f88192157b4eef5f).

* 2. Button 3 (thought of as Middle mouse or scroll wheel button)

#### `public int8_t `[`x`](#struct_relative_report_1afea516f255dade5afc23b45ba7d29eaf) 

This will always be in range -128 <= `x` <= 127.

#### `public int8_t `[`y`](#struct_relative_report_1a336a8acf2a0391127ed1c426e1118b83) 

This will always be in range -128 <= `y` <= 127.

#### `public int8_t `[`scroll`](#struct_relative_report_1a89f89a8919eaac29b04cbec6d9e6ddc2) 

This will always be in range -128 <= `scroll` <= 127.

In Relative/Mouse mode the scroll wheel data is only reported if the `intellimouse` parameter is passed as `true` to [PinnacleTouch::relativeModeConfig()](#class_pinnacle_touch_1abebd75f47e607943f88192157b4eef5f). Otherwise this is an empty byte as the [RelativeReport](#struct_relative_report) follows the buffer structure of a mouse HID report.

Generated by [Moxygen](https://sourcey.com/moxygen)