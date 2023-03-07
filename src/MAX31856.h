#ifndef __MAX31856_H
#define __MAX31856_H

#include <SPIDevice.h>

class MAX31856 : SPIDevice
{
    public:
        MAX31856(SPIClass& spi, const uint8_t pin_cs)
            : SPIDevice(spi, pin_cs, 1000000, SPI_MSBFIRST, SPI_MODE0) {};
        
        bool begin();

        typedef enum {
            MODE_ONESHOT,
            MODE_ONESHOT_NOWAIT,
            MODE_CONTINUOUS
        } conversion_mode_t;
        void setConversionMode(const conversion_mode_t mode);

        typedef enum {
            TCTYPE_B = 0b0000,
            TCTYPE_E = 0b0001,
            TCTYPE_J = 0b0010,
            TCTYPE_K = 0b0011,
            TCTYPE_N = 0b0100,
            TCTYPE_R = 0b0101,
            TCTYPE_S = 0b0110,
            TCTYPE_T = 0b0111,
            VMODE_G8 = 0b1000,
            VMODE_G32 = 0b1100,
        } thermocoupletype_t;
       void setThermocoupleType(const thermocoupletype_t type);

        typedef enum 
        {
            NOISE_FILTER_50HZ,
            NOISE_FILTER_60HZ
        } noise_filter_t;
        void setNoiseFilter(const noise_filter_t nf);

        uint8_t getFault(void);
        void setColdJunctionFaultThreshholds(const int8_t low, const int8_t high);
        void setTempFaultThreshholds(const float flow, const float fhigh);
        void triggerOneShot(void);
        bool conversionComplete(void);
        float readCJTemperature(void);
        float readThermocoupleTemperature(void);

    private:
        conversion_mode_t _conversion_mode = MODE_ONESHOT;

};

#endif // __MAX31856_H