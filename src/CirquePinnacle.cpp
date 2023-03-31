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
#include "CirquePinnacle.h"
#ifdef PINNACLE_SPI_BUFFER_OPS
    #include <cstdlib> // malloc(), free()
    #include <cstring> // memcpy(), memset()
#endif

PinnacleTouch::PinnacleTouch(pinnacle_gpio_t dataReadyPin) : _dataReady(dataReadyPin)
{
    PINNACLE_USE_ARDUINO_API
    if (_dataReady != PINNACLE_SW_DR) {
        pinMode(_dataReady, INPUT);
    }
}

bool PinnacleTouch::begin()
{
    PINNACLE_USE_ARDUINO_API
    delay(100);
    uint8_t firmware[2] = {};
    rapReadBytes(PINNACLE_FIRMWARE_ID, firmware, 2);
    if (firmware[0] == 7 || firmware[1] == 0x3A) {
        _dataMode = PINNACLE_RELATIVE;
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
        return calibrate(true); // enables all compensations, runs calibration, & clearStatusFlags()
    }
    // else if hardware check failed
    _dataMode = PINNACLE_ERROR; // prevent operations if hardware check failed
    return false;
}

void PinnacleTouch::feedEnabled(bool isEnabled)
{
    if (_dataMode == PINNACLE_ABSOLUTE || _dataMode == PINNACLE_RELATIVE) {
        uint8_t temp = 0;
        rapRead(PINNACLE_FEED_CONFIG_1, &temp);
        if (static_cast<bool>(temp & 1) != isEnabled)
            rapWrite(PINNACLE_FEED_CONFIG_1, (temp & 0xfe) | isEnabled);
    }
}

bool PinnacleTouch::isFeedEnabled()
{
    if (_dataMode == PINNACLE_ABSOLUTE || _dataMode == PINNACLE_RELATIVE) {
        uint8_t temp = 0;
        rapRead(PINNACLE_FEED_CONFIG_1, &temp);
        return static_cast<bool>(temp & 1);
    }
    /* AnyMeas mode: "feed" is instigated by measureADC()
       & x,y tracking measurements are already disabled */
    return false;
}

void PinnacleTouch::setDataMode(PinnacleDataMode mode)
{
    PINNACLE_USE_ARDUINO_API
    if (mode <= PINNACLE_ABSOLUTE && _dataMode != PINNACLE_ERROR) {
        uint8_t sysConfig = 0;
        rapRead(PINNACLE_SYS_CONFIG, &sysConfig);
        sysConfig &= 0xE7;
        if (mode == PINNACLE_RELATIVE || mode == PINNACLE_ABSOLUTE) {
#if PINNACLE_ANYMEAS_SUPPORT
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

#endif // PINNACLE_ANYMEAS_SUPPORT == true
                _dataMode = mode;
                rapWrite(PINNACLE_FEED_CONFIG_1, 1 | mode);
#if PINNACLE_ANYMEAS_SUPPORT
            }
        }
        else if (mode == PINNACLE_ANYMEAS && _dataReady != PINNACLE_SW_DR) { // DR_PIN is required for anymeas mode
            // disable tracking computations for AnyMeas mode
            rapWrite(PINNACLE_SYS_CONFIG, sysConfig | 0x08);
            delay(10); // wait 10 ms for tracking measurements to expire
            _dataMode = mode;
            anymeasModeConfig(); // configure registers for the AnyMeas mode

#endif // PINNACLE_ANYMEAS_SUPPORT == true
        }
    }
}

PinnacleDataMode PinnacleTouch::getDataMode()
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
    if (_dataReady == PINNACLE_SW_DR) {
        uint8_t tmp = 0;
        rapRead(PINNACLE_STATUS, &tmp);
        return (bool)(tmp & 0x0C);
    }
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

void PinnacleTouch::relativeModeConfig(bool taps, bool rotate90, bool secondaryTap, bool glideExtend, bool intellimouse)
{
    if (_dataMode == PINNACLE_RELATIVE) {
        uint8_t config2 = (rotate90 << 7) | (!glideExtend << 4) | (!secondaryTap << 2) | (!taps << 1) | intellimouse;
        rapWrite(PINNACLE_FEED_CONFIG_2, config2);
    }
}

void PinnacleTouch::read(RelativeReport* report)
{
    if (_dataMode == PINNACLE_RELATIVE) {
        uint8_t temp[4] = {};
        rapReadBytes(PINNACLE_PACKET_BYTE_0, temp, 4);
        clearStatusFlags();
        report->buttons &= 0xF8;
        report->buttons |= temp[0] & 7;
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
        report->buttons &= 0xF8;
        report->buttons |= temp[0] & 0x3F;
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
        rapWrite(PINNACLE_SAMPLE_RATE, (uint8_t)value);
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
        else if (temp != 10 || temp != 20 || temp != 40 || temp != 60 || temp != 80 || temp != 100) {
            return 100; // using default for unsupported input values
        }
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

bool PinnacleTouch::calibrate(bool run, bool tap, bool trackError, bool nerd, bool background)
{
    PINNACLE_USE_ARDUINO_API
    if (_dataMode == PINNACLE_ABSOLUTE || _dataMode == PINNACLE_RELATIVE) {
        uint8_t cal_config = (tap << 4) | (trackError << 3) | (nerd << 2) | (background << 1);
        rapWrite(PINNACLE_CAL_CONFIG, cal_config | run);
        if (run) {
            bool done = false;
            uint32_t timeout = millis() + 100;
            while (!done && millis() < timeout) { // calibration is running
                done = available();
            }
            if (done)
                clearStatusFlags(); // now that calibration is done
            return done;
        }
        return true;
    }
    return false;
}

void PinnacleTouch::setCalibrationMatrix(int16_t* matrix, uint8_t len)
{
    if (_dataMode <= PINNACLE_ABSOLUTE) {
        bool prevFeedState = isFeedEnabled();
        if (prevFeedState)
            feedEnabled(false); // this will save time on subsequent eraWrite calls

        for (uint8_t i = 0; i < 46; i++) {
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
        // must use sequential read of 92 bytes; individual reads return inaccurate data
        eraReadBytes(0x01DF, reinterpret_cast<uint8_t*>(matrix), 92);
        for (uint8_t i = 0; i < 46; ++i) {
            // reverse the endianess that was read from the registers
            matrix[i] = (matrix[i] << 8) | (matrix[i] >> 8);
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

#if PINNACLE_ANYMEAS_SUPPORT

void PinnacleTouch::anymeasModeConfig(uint8_t gain, uint8_t frequency, uint32_t sampleLength, uint8_t muxControl, uint32_t apertureWidth, uint8_t controlPowerCount)
{
    if (_dataMode == PINNACLE_ANYMEAS) {
        uint8_t togPol[8] = {0};
        rapWriteBytes(PINNACLE_PACKET_BYTE_1, togPol, 8);
        uint8_t anymeas_config[10] = {2, 3, 4, 0, 4, 0, PINNACLE_PACKET_BYTE_1, 0, 0, 1};
        anymeas_config[0] = gain | frequency;
        sampleLength /= 128;
        anymeas_config[1] = (uint8_t)(sampleLength < 1 ? 1 : (sampleLength > 3 ? 3 : sampleLength));
        anymeas_config[2] = muxControl;
        apertureWidth /= 125;
        anymeas_config[4] = (uint8_t)(apertureWidth < 2 ? 2 : (apertureWidth > 15 ? 15 : apertureWidth));
        anymeas_config[9] = controlPowerCount;
        rapWriteBytes(5, anymeas_config, 10);
        clearStatusFlags();
    }
}

int16_t PinnacleTouch::measureAdc(unsigned int bitsToToggle, unsigned int togglePolarity)
{
    startMeasureAdc(bitsToToggle, togglePolarity);
    while (!available() && _dataReady != PINNACLE_SW_DR) {
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

#endif // PINNACLE_ANYMEAS_SUPPORT == true

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
    do {
        rapRead(PINNACLE_ERA_CONTROL, &temp); // read until registerAddress == 0
    } while (temp);
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
        do {
            rapRead(PINNACLE_ERA_CONTROL, &temp); // read until registerAddress == 0
        } while (temp);
        clearStatusFlags(); // clear Command Complete flag in Status register
    }
    if (prevFeedState) {
        feedEnabled(prevFeedState); // resume previous feed state
    }
}

void PinnacleTouch::eraRead(uint16_t registerAddress, uint8_t* data)
{
    bool prevFeedState = isFeedEnabled();
    if (prevFeedState) {
        feedEnabled(false); // accessing raw memory, so do this
    }
    uint8_t reg_value[2] = {(uint8_t)(registerAddress >> 8), (uint8_t)(registerAddress & 0xff)};
    rapWriteBytes(PINNACLE_ERA_ADDR, reg_value, 2);
    rapWrite(PINNACLE_ERA_CONTROL, 1); // indicate reading only 1 byte
    uint8_t temp = 1;
    do {
        rapRead(PINNACLE_ERA_CONTROL, &temp); // read until registerAddress == 0
    } while (temp);
    rapRead(PINNACLE_ERA_VALUE, data); // get data
    clearStatusFlags();                // clear Command Complete flag in Status register
    if (prevFeedState) {
        feedEnabled(prevFeedState); // resume previous feed state
    }
}

void PinnacleTouch::eraReadBytes(uint16_t registerAddress, uint8_t* data, uint8_t registerCount)
{
    bool prevFeedState = isFeedEnabled();
    if (prevFeedState) {
        feedEnabled(false); // accessing raw memory, so do this
    }
    uint8_t reg_value[2] = {(uint8_t)(registerAddress >> 8), (uint8_t)(registerAddress & 0xff)};
    rapWriteBytes(PINNACLE_ERA_ADDR, reg_value, 2);
    uint8_t temp = 1;
    for (uint8_t i = 0; i < registerCount; ++i) {
        rapWrite(PINNACLE_ERA_CONTROL, 5); // indicate reading sequential bytes
        do {
            rapRead(PINNACLE_ERA_CONTROL, &temp); // read until registerAddress == 0
        } while (temp);
        rapRead(PINNACLE_ERA_VALUE, data + i); // get value
        clearStatusFlags();                    // clear Command Complete flag in Status register
    }
    if (prevFeedState) {
        feedEnabled(prevFeedState); // resume previous feed state
    }
}

#if PINNACLE_DEV_HW_DEBUG
void PinnacleTouch::readRegisters(uint8_t reg, uint8_t* data, uint8_t len)
{
    rapReadBytes(reg, data, len);
}
#endif

PinnacleTouchSPI::PinnacleTouchSPI(pinnacle_gpio_t dataReadyPin, pinnacle_gpio_t slaveSelectPin, uint32_t spiSpeed)
    : PinnacleTouch(dataReadyPin), _slaveSelect(slaveSelectPin), _spiSpeed(spiSpeed)
{
}

bool PinnacleTouchSPI::begin(pinnacle_spi_t* spi_bus)
{
    PINNACLE_USE_ARDUINO_API
    spi = spi_bus;
#ifndef PINNACLE_USE_NATIVE_CS
    pinMode(_slaveSelect, OUTPUT);
    PINNACLE_SS_CTRL(_slaveSelect, HIGH);
#endif
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
    spi->beginTransaction(SPISettings(_spiSpeed, MSBFIRST, SPI_MODE1));
#endif
    PINNACLE_SS_CTRL(_slaveSelect, LOW);
#ifdef PINNACLE_SPI_BUFFER_OPS
    uint8_t* buf = (uint8_t*)malloc(2);
    buf[0] = (uint8_t)(0x80 | registerAddress);
    buf[1] = registerValue;
    spi->transfer(buf, 2);
    free(buf);
#else  // !defined(PINNACLE_SPI_BUFFER_OPS)
    spi->transfer((uint8_t)(0x80 | registerAddress));
    spi->transfer(registerValue);
#endif // !defined(PINNACLE_SPI_BUFFER_OPS)
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
    spi->beginTransaction(SPISettings(_spiSpeed, MSBFIRST, SPI_MODE1));
#endif
    PINNACLE_SS_CTRL(_slaveSelect, LOW);
#ifdef PINNACLE_SPI_BUFFER_OPS
    uint8_t bufSize = 3 + registerCount;
    uint8_t* buf = (uint8_t*)malloc(bufSize);
    memset(buf, 0xFC, bufSize);
    buf[0] = 0xA0 | registerAddress;
    spi->transfer(buf, bufSize);
    memcpy(data, buf + 3, registerCount);
    free(buf);
#else  // !defined(PINNACLE_SPI_BUFFER_OPS)
    spi->transfer(0xA0 | registerAddress);
    spi->transfer(0xFC);
    spi->transfer(0xFC);
    for (uint8_t i = 0; i < registerCount; ++i)
        data[i] = spi->transfer(0xFC);
#endif // !defined(PINNACLE_SPI_BUFFER_OPS)
    PINNACLE_SS_CTRL(_slaveSelect, HIGH);
#ifdef SPI_HAS_TRANSACTION
    spi->endTransaction();
#endif
}

PinnacleTouchI2C::PinnacleTouchI2C(pinnacle_gpio_t dataReadyPin, uint8_t slaveAddress)
    : PinnacleTouch(dataReadyPin), _slaveAddress(slaveAddress)
{
}

bool PinnacleTouchI2C::begin(pinnacle_i2c_t* i2c_bus)
{
    i2c = i2c_bus;
    return PinnacleTouch::begin();
}

bool PinnacleTouchI2C::begin()
{
    PINNACLE_USE_ARDUINO_API
    Wire.begin();
    // no max/min I2C clock speed is specified (in ASIC's specs); use MCU default.
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
        i2c->write(0x80 | (registerAddress + i));
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
    i2c->requestFrom(_slaveAddress, registerCount, (uint8_t) true);
    while (i2c->available()) {
        data[i++] = i2c->read();
    }
}
