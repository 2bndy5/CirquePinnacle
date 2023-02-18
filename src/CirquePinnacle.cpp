/**
 *  @file CirquePinnacle.cpp
 *  A library to interface (via I2C or SPI protocol) with Cirque's Glidepoint circle
 *  trackpads that employ Cirque's Pinnacle ASIC touch controller (1CA027).
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
#include "CirquePinnacle.h"

PinnacleTouch::PinnacleTouch(pinnacle_gpio_t dataReadyPin)
{
    PINNACLE_USE_ARDUINO_API
    _dataReady = dataReadyPin;
    pinMode(dataReadyPin, INPUT);
}

bool PinnacleTouch::begin()
{
    uint8_t firmware[2] = {};
    rapReadBytes(PINNACLE_FIRMWARE_ID, firmware, 2);
    if (firmware[0] == 7 || firmware[1] == 0x3A) {
        _dataMode = static_cast<uint8_t>(0);
        clearStatusFlags();
        detectFingerStylus();          // detects both finger & stylus; sets sample rate to 100
        rapWrite(PINNACLE_Z_IDLE, 30); // 30 z-idle packets
        setAdcGain(0);                 // most sensitive attenuation
        tuneEdgeSensitivity();         // because "why not?"
        uint8_t configs[3] = {0, 1, 0};
        // configs[0] => clears AnyMeas flags
        // configs[1] => set relative mode & enable feed
        // configs[2] => enables all taps in Relative mode
        rapWriteBytes(PINNACLE_SYS_CONFIG, configs, 3);
        calibrate(true); // enables all compensations, runs calibration, & clearStatusFlags()
        return true;
    } // hardware check passed
    else {
        _dataMode = static_cast<uint8_t>(0xFF); // prevent operations if hardware check failed
        return false;
    }
}

void PinnacleTouch::feedEnabled(bool isEnabled)
{
    if (_dataMode == PINNACLE_ABSOLUTE || _dataMode == PINNACLE_RELATIVE) {
        uint8_t temp = 0;
        rapRead(PINNACLE_FEED_CONFIG_1, &temp);
        if ((bool)(temp & 1) != isEnabled)
            rapWrite(PINNACLE_FEED_CONFIG_1, (temp & 0xfe) | isEnabled);
    }
}

bool PinnacleTouch::isFeedEnabled()
{
    if (_dataMode == PINNACLE_ABSOLUTE || _dataMode == PINNACLE_RELATIVE) {
        uint8_t temp = 0;
        rapRead(PINNACLE_FEED_CONFIG_1, &temp);
        return (bool)(temp & 1);
    }
    /*  AnyMeas mode: "feed" is instigated by measureADC()
    & x,y tracking measurements are already disabled*/
    return false;
}

void PinnacleTouch::setDataMode(PinnacleDataMode mode)
{
    PINNACLE_USE_ARDUINO_API
    if (mode <= 2) {
        uint8_t sysConfig = 0;
        rapRead(PINNACLE_SYS_CONFIG, &sysConfig);
        sysConfig &= 0xE7;
        if (mode == PINNACLE_RELATIVE || mode == PINNACLE_ABSOLUTE) {
#ifdef PINNACLE_ANYMEAS_SUPPORT
            if (_dataMode == PINNACLE_ANYMEAS) { // if leaving AnyMeas mode
                _dataMode = mode;
                uint8_t configs[3] = {sysConfig, static_cast<uint8_t>(_dataMode | 1), 0};
                // configs[0] => clears AnyMeas flags
                // configs[1] => set new mode's flag & enables feed
                // configs[2] => enables taps in Relative mode
                rapWriteBytes(PINNACLE_SYS_CONFIG, configs, 3);
                setSampleRate(100);
                rapWrite(PINNACLE_CAL_CONFIG, 0x1E); // enables all compensations
                rapWrite(PINNACLE_Z_IDLE, 30);       // 30 z-idle packets
            }
            else { // ok to just write appropriate mode

#endif // !defined(PINNACLE_ANYMEAS_SUPPORT)
                _dataMode = mode;
                rapWrite(PINNACLE_FEED_CONFIG_1, 1 | mode);
#ifdef PINNACLE_ANYMEAS_SUPPORT
            }
        }
        else if (mode == PINNACLE_ANYMEAS) {
            // disable tracking computations for AnyMeas mode
            rapWrite(PINNACLE_SYS_CONFIG, sysConfig | 0x08);
            delay(10);           // wait 10 ms for tracking measurements to expire
            anymeasModeConfig(); // configure registers for the AnyMeas mode

#endif // !defined(PINNACLE_ANYMEAS_SUPPORT)
        }
    }
}

uint8_t PinnacleTouch::getDataMode()
{
    return _dataMode;
}

bool PinnacleTouch::isHardConfigured()
{
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        uint8_t temp = 0;
        rapRead(PINNACLE_HCO_ID, &temp);
        return temp > 0;
    }
    return false;
}

bool PinnacleTouch::available()
{
    PINNACLE_USE_ARDUINO_API
    return digitalRead(_dataReady);
}

void PinnacleTouch::absoluteModeConfig(uint8_t zIdleCount, bool invertX, bool invertY)
{
    if (_dataMode == PINNACLE_ABSOLUTE) {
        rapWrite(PINNACLE_Z_IDLE, zIdleCount);
        uint8_t temp = 0;
        rapRead(PINNACLE_FEED_CONFIG_1, &temp);
        rapWrite(PINNACLE_FEED_CONFIG_1, (temp & 0x3F) | (invertY << 7) | (invertX << 6));
    }
}

void PinnacleTouch::relativeModeConfig(bool rotate90, bool allTaps, bool secondaryTap, bool glideExtend, bool intellimouse)
{
    if (_dataMode == PINNACLE_RELATIVE) {
        uint8_t config2 = (rotate90 << 7) | (!glideExtend << 4) | (!secondaryTap << 2) | (!allTaps << 1) | intellimouse;
        rapWrite(PINNACLE_FEED_CONFIG_2, config2);
    }
}

void PinnacleTouch::read(RelativeReport* report)
{
    if (_dataMode == PINNACLE_RELATIVE) {
        uint8_t temp[4] = {};
        rapReadBytes(PINNACLE_PACKET_BYTE_0, temp, 4);
        clearStatusFlags();
        report->buttons = temp[0] & 7;
        report->x = (int8_t)temp[1];
        report->y = (int8_t)temp[2];
        report->scroll = (int8_t)temp[3];
    }
}

void PinnacleTouch::read(AbsoluteReport* report)
{
    if (_dataMode == PINNACLE_ABSOLUTE) {
        uint8_t temp[6] = {};
        rapReadBytes(PINNACLE_PACKET_BYTE_0, temp, 6);
        clearStatusFlags();
        report->buttons = temp[0] & 0x3F;
        report->x = (uint16_t)(((temp[4] & 0x0F) << 8) | temp[2]);
        report->y = (uint16_t)(((temp[4] & 0xF0) << 4) | temp[3]);
        report->z = (uint8_t)(temp[5] & 0x1F);
    }
}

void PinnacleTouch::clearStatusFlags()
{
    PINNACLE_USE_ARDUINO_API
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        rapWrite(PINNACLE_STATUS, 0);
        delayMicroseconds(50);
    }
}

void PinnacleTouch::allowSleep(bool isEnabled)
{
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        uint8_t temp = 0;
        rapRead(PINNACLE_SYS_CONFIG, &temp);
        rapWrite(PINNACLE_SYS_CONFIG, (temp & 0xFB) | (isEnabled << 2));
    }
}

bool PinnacleTouch::isAllowSleep()
{
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        uint8_t temp = 0;
        rapRead(PINNACLE_SYS_CONFIG, &temp);
        return (bool)(temp & 4);
    }
    return false;
}

void PinnacleTouch::shutdown(bool isOff)
{
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        uint8_t temp = 0;
        rapRead(PINNACLE_SYS_CONFIG, &temp);
        temp &= 0xFD;
        rapWrite(PINNACLE_SYS_CONFIG, temp | (isOff << 1));
    }
}

bool PinnacleTouch::isShutdown()
{
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        uint8_t temp = 0;
        rapRead(PINNACLE_SYS_CONFIG, &temp);
        return (bool)(temp & 2);
    }
    return false;
}

void PinnacleTouch::setSampleRate(uint16_t value)
{
    if (_dataMode == PINNACLE_ABSOLUTE || _dataMode == PINNACLE_RELATIVE) {
        if (value == 200 || value == 300) {
            // disable palm & noise compensations
            rapWrite(PINNACLE_FEED_CONFIG_3, 10);
            uint8_t reloadTimer = value == 300 ? 6 : 9;
            eraWriteBytes(0x019E, reloadTimer, 2);
            value = 0;
        }
        else {
            // enable palm & noise compensations
            rapWrite(PINNACLE_FEED_CONFIG_3, 0);
            eraWriteBytes(0x019E, 0x13, 2);
        }
        // bad input values interpreted as 100 by Pinnacle
        rapWrite(PINNACLE_SAMPLE_RATE, value);
    }
}

uint16_t PinnacleTouch::getSampleRate()
{
    if (_dataMode == PINNACLE_ABSOLUTE || _dataMode == PINNACLE_RELATIVE) {
        uint8_t temp = 0;
        rapRead(PINNACLE_SAMPLE_RATE, &temp);
        if (temp == 0) {
            eraRead(0x019E, &temp);
            return temp == 6 ? 300 : 200;
        }
        else
            return temp;
    }
    return 0; // "sample rate" = frequent calling of measureADC()
}

void PinnacleTouch::detectFingerStylus(bool enableFinger, bool enableStylus, uint16_t sampleRate)
{
    if (_dataMode == PINNACLE_ABSOLUTE || _dataMode == PINNACLE_RELATIVE) {
        setSampleRate(sampleRate);
        uint8_t fingerStylus = 0;
        eraRead(0x00EB, &fingerStylus);
        fingerStylus |= (enableStylus << 2) | enableFinger;
        eraWrite(0x00EB, fingerStylus);
    }
}

void PinnacleTouch::calibrate(bool run, bool tap, bool trackError, bool nerd, bool background)
{
    if (_dataMode == PINNACLE_ABSOLUTE || _dataMode == PINNACLE_RELATIVE) {
        uint8_t cal_config = (tap << 4) | (trackError << 3) | (nerd << 2) | (background << 1);
        rapWrite(PINNACLE_CAL_CONFIG, cal_config | run);
        if (run) {
            uint8_t temp = 1;
            while (temp & 1)
                rapRead(PINNACLE_CAL_CONFIG, &temp); // calibration is running

            clearStatusFlags(); // now that calibration is done
        }
    }
}

void PinnacleTouch::setCalibrationMatrix(int16_t* matrix, uint8_t len)
{
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        bool prevFeedState = isFeedEnabled();
        if (prevFeedState)
            feedEnabled(false); // this will save time on subsequent eraWrite calls

        for (uint8_t i = 0; i < 46; i++) { // truncate malformed matrices
            if (i < len) {
                eraWrite(0x01DF + i * 2, (uint8_t)(matrix[i] >> 8));
                eraWrite(0x01E0 + i * 2, (uint8_t)(matrix[i] & 0xFF));
            }
            else // pad out malformed matrices
                eraWriteBytes(0x01DF + i * 2, 0, 2);
        }

        if (prevFeedState)
            feedEnabled(prevFeedState); // resume previous feed state
    }
}

void PinnacleTouch::getCalibrationMatrix(int16_t* matrix)
{
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        uint8_t data[92] = {};
        eraReadBytes(0x01DF, data, 92);
        for (uint8_t i = 0; i < 92; i += 2) {
            matrix[i / 2] = (int16_t)data[i] << 8;
            matrix[i / 2] |= (int16_t)(data[i + 1]);
        }
    }
}

void PinnacleTouch::setAdcGain(uint8_t sensitivity)
{
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        if (sensitivity >= 4)
            sensitivity = 0; // faulty input defaults to highest sensitivity
        uint8_t temp = 0;
        eraRead(0x0187, &temp);
        eraWrite(0x0187, (temp & 0x3F) | (sensitivity << 6));
    }
}

void PinnacleTouch::tuneEdgeSensitivity(uint8_t xAxisWideZMin, uint8_t yAxisWideZMin)
{
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        eraWrite(0x0149, xAxisWideZMin);
        eraWrite(0x0168, yAxisWideZMin);
    }
}

#ifdef PINNACLE_ANYMEAS_SUPPORT

void PinnacleTouch::anymeasModeConfig(uint8_t gain, uint8_t frequency, uint32_t sampleLength, uint8_t muxControl, uint32_t apertureWidth, uint8_t controlPowerCount)
{
    if (_dataMode == PINNACLE_ANYMEAS)
    {
        uint8_t anymeas_config[10] = {2, 3, 4, 0, 4, 0, PINNACLE_PACKET_BYTE_1, 0, 0, 1};
        anymeas_config[0] = gain | frequency;
        sampleLength /= 128;
        anymeas_config[1] = (uint8_t)(sampleLength < 1 ? 1 : (sampleLength > 3 ? 3 : sampleLength));
        anymeas_config[2] = muxControl;
        apertureWidth /= 125;
        anymeas_config[4] = (uint8_t)(apertureWidth < 2 ? 2 : (apertureWidth > 15 ? 15 : apertureWidth));
        anymeas_config[9] = controlPowerCount;
        rapWriteBytes(5, anymeas_config, 10);
        uint8_t togPol[8] = {};
        rapWriteBytes(PINNACLE_PACKET_BYTE_1, togPol, 8);
        clearStatusFlags();
    }
}

int16_t PinnacleTouch::measureAdc(unsigned int bitsToToggle, unsigned int togglePolarity)
{
    startMeasureAdc(bitsToToggle, togglePolarity);
    while (!available()) {
        // wait till measurements are complete
    }
    return getMeasureAdc();
}

void PinnacleTouch::startMeasureAdc(unsigned int bitsToToggle, unsigned int togglePolarity)
{
    if (_dataMode == PINNACLE_ANYMEAS) {
        uint8_t togPol[8] = {}; // array buffer for registers
        for (int8_t i = 3; i >= 0; --i)
            togPol[3 - i] = (uint8_t)(bitsToToggle >> ((uint8_t)i * 8));
        for (int8_t i = 3; i >= 0; --i)
            togPol[3 - i + 4] = (uint8_t)(togglePolarity >> ((uint8_t)i * 8));
        rapWriteBytes(PINNACLE_PACKET_BYTE_1, togPol, 8);
        // initiate measurements
        uint8_t temp = 0;
        rapRead(PINNACLE_SYS_CONFIG, &temp);
        rapWrite(PINNACLE_SYS_CONFIG, temp | 0x18);
    }
}

int16_t PinnacleTouch::getMeasureAdc()
{
    if (_dataMode == PINNACLE_ANYMEAS && available()) {
        uint8_t result[2] = {};
        rapReadBytes(PINNACLE_PACKET_BYTE_0 - 1, result, 2);
        clearStatusFlags();
        return (int16_t)(((uint16_t)result[0] << 8) | result[1]);
    }
    return 0;
}

#endif // !defined(PINNACLE_ANYMEAS_SUPPORT)

void PinnacleTouch::eraWrite(uint16_t registerAddress, uint8_t registerValue)
{
    bool prevFeedState = isFeedEnabled();
    if (prevFeedState) {
        feedEnabled(false); // accessing raw memory, so do this
    }
    rapWrite(PINNACLE_ERA_VALUE, registerValue);
    uint8_t reg_value[2] = {(uint8_t)(registerAddress >> 8), (uint8_t)(registerAddress & 0xff)};
    rapWriteBytes(PINNACLE_ERA_ADDR, reg_value, 2);
    rapWrite(PINNACLE_ERA_CONTROL, 2); // indicate writing only 1 byte
    uint8_t temp = 1;
    while (temp) {
        rapRead(PINNACLE_ERA_CONTROL, &temp); // read until registerValue == 0
    }
    clearStatusFlags(); // clear Command Complete flag in Status register
    if (prevFeedState) {
        feedEnabled(prevFeedState); // resume previous feed state
    }
}

void PinnacleTouch::eraWriteBytes(uint16_t registerAddress, uint8_t registerValue, uint8_t repeat)
{
    // NOTE this is rarely used as it only writes 1 value to multiple registers
    bool prevFeedState = isFeedEnabled();
    if (prevFeedState) {
        feedEnabled(false); // accessing raw memory, so do this
    }
    rapWrite(PINNACLE_ERA_VALUE, registerValue);
    uint8_t reg_value[2] = {(uint8_t)(registerAddress >> 8), (uint8_t)(registerAddress & 0xff)};
    rapWriteBytes(PINNACLE_ERA_ADDR, reg_value, 2);
    rapWrite(PINNACLE_ERA_CONTROL, 0x0A); // indicate writing sequential bytes
    uint8_t temp = 1;
    for (uint8_t i = 0; i < repeat; i++) {
        while (temp) {
            rapRead(PINNACLE_ERA_CONTROL, &temp); // read until registerValue == 0
        }
        clearStatusFlags(); // clear Command Complete flag in Status register
    }
    if (prevFeedState) {
        feedEnabled(prevFeedState); // resume previous feed state
    }
}

void PinnacleTouch::eraRead(uint16_t registerAddress, uint8_t* data)
{
    bool prevFeedState = isFeedEnabled();
    if (prevFeedState)
    {
        feedEnabled(false); // accessing raw memory, so do this
    }
    uint8_t reg_value[2] = {(uint8_t)(registerAddress >> 8), (uint8_t)(registerAddress & 0xff)};
    rapWriteBytes(PINNACLE_ERA_ADDR, reg_value, 2);
    rapWrite(PINNACLE_ERA_CONTROL, 1); // indicate reading only 1 byte
    uint8_t temp = 1;
    while (temp)
    {
        rapRead(PINNACLE_ERA_CONTROL, &temp); // read until registerAddress == 0
    }
    rapRead(PINNACLE_ERA_VALUE, data); // get data
    clearStatusFlags();                // clear Command Complete flag in Status register
    if (prevFeedState)
    {
        feedEnabled(prevFeedState); // resume previous feed state
    }
}

void PinnacleTouch::eraReadBytes(uint16_t registerAddress, uint8_t* data, uint8_t registerCount)
{
    bool prevFeedState = isFeedEnabled();
    if (prevFeedState)
    {
        feedEnabled(false); // accessing raw memory, so do this
    }
    uint8_t reg_value[2] = {(uint8_t)(registerAddress >> 8), (uint8_t)(registerAddress & 0xff)};
    rapWriteBytes(PINNACLE_ERA_ADDR, reg_value, 2);
    rapWrite(PINNACLE_ERA_CONTROL, 1); // indicate reading only 1 byte
    uint8_t temp = 1;
    for (uint8_t i = 0; i < registerCount; i++)
    {
        while (temp)
        {
            rapRead(PINNACLE_ERA_CONTROL, &temp); // read until registerAddress == 0
        }
        rapRead(PINNACLE_ERA_VALUE, &data[i]); // get value
        clearStatusFlags();                    // clear Command Complete flag in Status register
    }
    if (prevFeedState)
    {
        feedEnabled(prevFeedState); // resume previous feed state
    }
}

#if PINNACLE_DEV_HW_DEBUG
void PinnacleTouch::readRegisters(uint8_t reg, uint8_t* data, uint8_t len)
{
    rapReadBytes(reg, data, len);
}
#endif

PinnacleTouchSPI::PinnacleTouchSPI(pinnacle_gpio_t dataReadyPin, pinnacle_gpio_t slaveSelectPin) : PinnacleTouch(dataReadyPin)
{
    _slaveSelect = slaveSelectPin;
}

bool PinnacleTouchSPI::begin(_SPI* spi_bus)
{
    PINNACLE_USE_ARDUINO_API
    spi = spi_bus;
    pinMode(_slaveSelect, OUTPUT);
    PINNACLE_SS_CTRL(_slaveSelect, HIGH);
    return PinnacleTouch::begin();
}

bool PinnacleTouchSPI::begin()
{
    PINNACLE_USE_ARDUINO_API
    SPI.begin();
    return PinnacleTouchSPI::begin(&SPI);
}

void PinnacleTouchSPI::rapWrite(uint8_t registerAddress, uint8_t registerValue)
{
    PINNACLE_USE_ARDUINO_API
#ifdef SPI_HAS_TRANSACTION
    spi->beginTransaction(SPISettings(PINNACLE_SPI_SPEED, MSBFIRST, SPI_MODE1));
#endif
    PINNACLE_SS_CTRL(_slaveSelect, LOW);
    spi->transfer((uint8_t)(0x80 | registerAddress));
    spi->transfer(registerValue);
    PINNACLE_SS_CTRL(_slaveSelect, HIGH);
#ifdef SPI_HAS_TRANSACTION
    spi->endTransaction();
#endif
}

void PinnacleTouchSPI::rapWriteBytes(uint8_t registerAddress, uint8_t* registerValues, uint8_t registerCount)
{
    for (uint8_t i = 0; i < registerCount; ++i)
        rapWrite(registerAddress + i, registerValues[i]);
}

void PinnacleTouchSPI::rapRead(uint8_t registerAddress, uint8_t* data)
{
    rapReadBytes(registerAddress, data, 1);
}

void PinnacleTouchSPI::rapReadBytes(uint8_t registerAddress, uint8_t* data, uint8_t registerCount)
{
    PINNACLE_USE_ARDUINO_API
#ifdef SPI_HAS_TRANSACTION
    spi->beginTransaction(SPISettings(PINNACLE_SPI_SPEED, MSBFIRST, SPI_MODE1));
#endif
    PINNACLE_SS_CTRL(_slaveSelect, LOW);
    spi->transfer(0xA0 | registerAddress);
    spi->transfer(0xFC);
    spi->transfer(0xFC);
    for (uint8_t i = 0; i < registerCount; ++i)
        data[i] = spi->transfer(0xFC);
    PINNACLE_SS_CTRL(_slaveSelect, HIGH);
#ifdef SPI_HAS_TRANSACTION
    spi->endTransaction();
#endif
}

PinnacleTouchI2C::PinnacleTouchI2C(pinnacle_gpio_t dataReadyPin, uint8_t slaveAddress) : PinnacleTouch(dataReadyPin)
{
    _slaveAddress = slaveAddress;
}

bool PinnacleTouchI2C::begin(_I2C* i2c_bus)
{
    i2c = i2c_bus;
    return PinnacleTouch::begin();
}

bool PinnacleTouchI2C::begin()
{
    PINNACLE_USE_ARDUINO_API
    Wire.begin();
    // no max/min I2C clock speed is specified (in ASIC's specs); use MCU's default.
    return PinnacleTouchI2C::begin(&Wire);
}

void PinnacleTouchI2C::rapWrite(uint8_t registerAddress, uint8_t registerValue)
{
    i2c->beginTransmission(_slaveAddress);
    i2c->write(0x80 | registerAddress);
    i2c->write(registerValue);
    i2c->endTransmission(true);
}

void PinnacleTouchI2C::rapWriteBytes(uint8_t registerAddress, uint8_t* registerValues, uint8_t registerCount)
{
    i2c->beginTransmission(_slaveAddress);
    for (uint8_t i = 0; i < registerCount; ++i) {
        i2c->write(0x80 | registerAddress);
        i2c->write(registerValues[i]);
    }
    i2c->endTransmission(true);
}

void PinnacleTouchI2C::rapRead(uint8_t registerAddress, uint8_t* data)
{
    rapReadBytes(registerAddress, data, 1);
}

void PinnacleTouchI2C::rapReadBytes(uint8_t registerAddress, uint8_t* data, uint8_t registerCount)
{
    uint8_t i = 0;
    i2c->beginTransmission(_slaveAddress);
    i2c->write(0xA0 | registerAddress);
    i2c->endTransmission(true);
    i2c->requestFrom((uint8_t)(_slaveAddress | 1), (uint8_t)registerCount, (uint8_t) true);
    while (i2c->available()) {
        data[i++] = i2c->read();
    }
}
