#include "MAX31856.h"
#include "MAX31856_regs.h"

#include <stdlib.h>

// #include <tools-log.h>

bool MAX31856::begin()
{
    SPIDevice::begin();

    // Check if the chip is there
    writereg8(REG_FMASK, 0xFF); // do not trigger new faults
    writereg8(REG_CONFIG0, CONFIG0_FAULTCLR); // clear faults
    if(readreg8(REG_STATUS) != 0x00) // should be zero
        return false;

    // assert on any fault
    writereg8(REG_FMASK, 0x00);

    // Start with some defaults to CONFIG0
    writereg8(REG_CONFIG0, REG_CONFIG0_DEFAULTS);

    // Set other defaults
    setColdJunctionOffset(0);
    setColdJunctionFaultThreshholds(-55, 125);
    setThermocoupleType(TCTYPE_K);
    setConversionMode(MODE_ONESHOT);

    return true;
};

void MAX31856::setConversionMode(const conversion_mode_t mode) 
{
    // get current CONFIG0
    uint8_t config0 = readreg8(REG_CONFIG0);
    
    config0 &= ~(CONFIG0_AUTOCONVERT | CONFIG0_1SHOT);
    switch(mode)
    {
        case MODE_ONESHOT_NOWAIT:
        case MODE_ONESHOT: config0 |= CONFIG0_AUTOCONVERT; break;

        case MODE_CONTINUOUS: config0 |= CONFIG0_1SHOT; break;

    };
    writereg8(REG_CONFIG0, config0);
    _conversion_mode = mode;
};

void MAX31856::setThermocoupleType(const thermocoupletype_t type) 
{
  uint8_t config1 = readreg8(REG_CONFIG1);
  config1 &= 0xF0; 
  config1 |= ((uint8_t) type & 0x0F);
  writereg8(REG_CONFIG1, config1);
};

void MAX31856::setNoiseFilter(noise_filter_t nf) 
{
  uint8_t config0 = readreg8(REG_CONFIG0);
  switch(nf)
  {
    case NOISE_FILTER_50HZ: config0 |= CONFIG0_50HZ; break;
    case NOISE_FILTER_60HZ: config0 &= ~CONFIG0_50HZ; break;
  };
  writereg8(REG_CONFIG0, config0);
};

uint8_t MAX31856::getFault(void) 
{
   return readreg8(REG_STATUS);
};

void MAX31856::setColdJunctionOffset(const int8_t t)
{
    writereg8(REG_CJTO, t);
};

/**************************************************************************/
/*!
    @brief  Sets the threshhold for internal chip temperature range
    for fault detection. NOT the thermocouple temperature range!
    @param  low Low (min) temperature, signed 8 bit so -128 to 127 degrees C
    @param  high High (max) temperature, signed 8 bit so -128 to 127 degrees C
*/
/**************************************************************************/
void MAX31856::setColdJunctionFaultThreshholds(const int8_t low, const int8_t high) 
{
  writereg8(REG_CJLF, low);
  writereg8(REG_CJHF, high);
};

/**************************************************************************/
/*!
    @brief  Sets the threshhold for thermocouple temperature range
    for fault detection. NOT the internal chip temperature range!
    @param  flow Low (min) temperature, floating point
    @param  fhigh High (max) temperature, floating point
*/
/**************************************************************************/
void MAX31856::setTempFaultThreshholds(const float flow, const float fhigh) 
{
  uint16_t low = flow * 16;

  uint16_t high = fhigh * 16;

  writereg8(REG_LTHFTH, high >> 8);
  writereg8(REG_LTHFTL, high);
  writereg8(REG_LTLFTH, low >> 8);
  writereg8(REG_LTLFTL, low);
};

/**************************************************************************/
/*!
    @brief  Begin a one-shot (read temperature only upon request) measurement.
    Value must be read later, not returned here!
*/
/**************************************************************************/
void MAX31856::triggerOneShot() 
{
  if(_conversion_mode == MODE_CONTINUOUS)
    return;

  uint8_t config0 = readreg8(REG_CONFIG0);
  config0 &= ~CONFIG0_AUTOCONVERT;
  config0 |=  CONFIG0_1SHOT;
  writereg8(REG_CONFIG0, config0);
  // conversion starts when CS goes high
};

/**************************************************************************/
/*!
    @brief  Return status of temperature conversion.
    @returns true if conversion complete, otherwise false
*/
/**************************************************************************/
bool MAX31856::conversionComplete() 
{
  if(_conversion_mode == MODE_CONTINUOUS)
    return true;
  uint8_t config0 = readreg8(REG_CONFIG0);
  return !(config0 & CONFIG0_1SHOT);
};

// /**************************************************************************/
// /*!
//     @brief  Return cold-junction (internal chip) temperature
//     @returns Floating point temperature in Celsius
// */
// /**************************************************************************/
float MAX31856::readCJTemperature() 
{
   return readreg16(REG_CJTH) / 256.0;
};

/**************************************************************************/
/*!
    @brief  Return hot-junction (thermocouple) temperature
    @returns Floating point temperature in Celsius
*/
/**************************************************************************/
float MAX31856::readThermocoupleTemperature() 
{
  // for one-shot, make it happen
  if(_conversion_mode == MODE_ONESHOT)
  {
    triggerOneShot();
    time_t start = millis();
    while (!conversionComplete()) 
    {
      if (millis() - start > 250) // takes about 185ms
        return NAN;
      delay(5);
    };
    // time_t end = millis();
    // DBG("Conversion complete after %d ms", end - start);
  };

  // read the thermocouple temperature registers (3 bytes)
  int32_t temp24 = readreg24(REG_LTCBH);

  // extend sign to 32-bit
  if (temp24 & 0x00800000) 
    temp24 |=  0xFF000000;
  temp24 >>= 5; // bottom 5 bits are unused, sign bit is extended

  return temp24 * REG_LTCBx_LSB;
};

