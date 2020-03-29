/*
    CirquePinnacle.h
        A library to interface (via I2C or SPI protocol) with Cirque's Glidepoint circle
        touchpads that employ Cirque's Pinnacle ASIC touch controller (1CA027).

        Store links (where to buy):
        Individual trackpads ==> https://www.mouser.com/Search/Refine?Ntk=P_MarCom&Ntt=118816186
        developer kits ==> https://www.mouser.com/Search/Refine?Ntk=P_MarCom&Ntt=183712866

    Written by Brendan Doherty to include all functionality demonstrated by
    the official Cirque github repository based on the teensy3.2 and the
    Cirque Glidepoint circle trackpad developer kit.
        Cirque example repository ==> https://github.com/cirque-corp/Cirque_Pinnacle_1CA027

    License and copyright information is located at this repository's root
    directory under LICENSE.txt
*/
#ifndef CirquePinnacle_H
#define CirquePinnacle_H
#include <stdint.h>

// defined Constants for bitwise configuration
#define PINNACLE_RELATIVE          0x00
#define PINNACLE_ANYMEAS           0x01
#define PINNACLE_ABSOLUTE          0x02
#define PINNACLE_GAIN_100          0xC0
#define PINNACLE_GAIN_133          0x80
#define PINNACLE_GAIN_166          0x40
#define PINNACLE_GAIN_200          0x00
#define PINNACLE_FREQ_0            0x02
#define PINNACLE_FREQ_1            0x03
#define PINNACLE_FREQ_2            0x04
#define PINNACLE_FREQ_3            0x05
#define PINNACLE_FREQ_4            0x06
#define PINNACLE_FREQ_5            0x07
#define PINNACLE_FREQ_6            0x09
#define PINNACLE_FREQ_7            0x0B
#define PINNACLE_MUX_REF1          0x10
#define PINNACLE_MUX_REF0          0x08
#define PINNACLE_MUX_PNP           0x04
#define PINNACLE_MUX_NPN           0x01
#define PINNACLE_CRTL_REPEAT       0x80
#define PINNACLE_CRTL_PWR_IDLE     0x40

// defined constants for Pinnacle registers
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

// data structure for data reports in relative mode
typedef struct _RelativeReport{
    uint8_t buttons;
    int8_t x;
    int8_t y;
    int8_t scroll;
} relativeReport;

// data structure for data reports in absolute mode
typedef struct _AbsoluteReport{
    uint8_t buttons;
    uint16_t x;
    uint16_t y;
    uint8_t z;
} absoluteReport;

class PinnacleTouch{
public:
    PinnacleTouch(uint8_t dataReadyPin);
    void feedEnabled(bool);
    bool isFeedEnabled();
    void setDataMode(uint8_t);
    uint8_t getDataMode();
    bool isHardConfigured();
    bool available();
    void absoluteModeConfig(uint8_t zIdleCount=30,
                            bool invertX=false,
                            bool invertY=false);
    void relativeModeConfig(bool rotate90=false,
                            bool glideExtend=false,
                            bool secondaryTap=true,
                            bool allTaps=false,
                            bool intellimouse=false);
    relativeReport reportRelative(bool onlyNew=true);
    absoluteReport reportAbsolute(bool onlyNew=true);
    void clearFlags();
    void setAllowSleep(bool);
    bool getAllowSleep();
    void shutdown(bool);
    bool isShutdown();
    void setSampleRate(uint16_t);
    uint16_t getSampleRate();
    void detectFingerStylus(bool enableFinger=true,
                            bool enableStylus=true,
                            uint16_t sampleRate=100);
    void calibrate( bool run,
                    bool tap=true,
                    bool trackError=true,
                    bool nerd=true,
                    bool background=true);
    void setCalibrationMatrix(int16_t*);
    int16_t* getCalibrationMatrix();
    void setAdcGain(uint8_t);
    void tuneEdgeSensitivity(uint8_t xAxisWideZMin=4, uint8_t yAxisWideZMin=3);
    void anyMeasModeConfig( uint8_t gain=PINNACLE_GAIN_200,
                            uint8_t frequency=PINNACLE_FREQ_0,
                            uint32_t sampleLength=512,
                            uint8_t muxControl=PINNACLE_MUX_PNP,
                            uint8_t appertureWidth=500,
                            uint8_t controlPowerCount=1);
    int16_t measureADC(unsigned int, unsigned int);
private:
    void eraWrite(uint16_t, uint8_t);
    void eraWriteBytes(uint16_t, uint8_t, uint8_t);
    uint8_t eraRead(uint16_t);
    uint8_t* eraReadBytes(uint16_t, uint8_t);
protected:
    bool begin();
    virtual void rapWrite(uint8_t, uint8_t);
    virtual void rapWriteBytes(uint8_t, uint8_t*, uint8_t);
    virtual uint8_t rapRead(uint8_t);
    virtual uint8_t* rapReadBytes(uint8_t, uint8_t);
    uint8_t dataReady;
    uint8_t dataMode;
};

class PinnacleTouchSPI: public PinnacleTouch{
public:
    PinnacleTouchSPI(uint8_t dataReadyPin, uint8_t slaveSelectPin);
    bool begin();
private:
    void rapWrite(uint8_t, uint8_t);
    void rapWriteBytes(uint8_t, uint8_t*, uint8_t);
    uint8_t rapRead(uint8_t);
    uint8_t* rapReadBytes(uint8_t, uint8_t);
    uint8_t _slaveSelect;
};

class PinnacleTouchI2C: public PinnacleTouch{
public:
    PinnacleTouchI2C(uint8_t dataReadyPin, uint8_t slaveAddress=0x2A);
    bool begin();
private:
    void rapWrite(uint8_t, uint8_t);
    void rapWriteBytes(uint8_t, uint8_t*, uint8_t);
    uint8_t rapRead(uint8_t);
    uint8_t* rapReadBytes(uint8_t, uint8_t);
    uint8_t _slaveAddress;
};
#endif
