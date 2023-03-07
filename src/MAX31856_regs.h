#ifndef __MAX31856_REGS_H
#define __MAX31856_REGS_H

#define REG_CONFIG0                 0x00
  #define CONFIG0_50HZ              0x01 // 50 Hz mode
  #define CONFIG0_FAULTCLR          0x02 // Config 0 fault clear flag
  #define CONFIG0_FAULT             0x04 // Config 0 fault mode flag
  #define CONFIG0_CJ                0x08 // Config 0 cold junction disable flag
  #define CONFIG0_OCFAULT0          0x10 // Config 0 open circuit fault 0 flag
  #define CONFIG0_OCFAULT1          0x20 // Config 0 open circuit fault 1 flag
  #define CONFIG0_1SHOT             0x40 // Config 0 one shot convert flag
  #define CONFIG0_AUTOCONVERT       0x80 // Config 0 Auto convert flag

#define REG_CONFIG1                 0x01
#define REG_FMASK                   0x02 // Fault Mask register
#define REG_CJHF                    0x03 // Cold junction High temp fault register
#define REG_CJLF                    0x04 // Cold junction Low temp fault register
#define REG_LTHFTH                  0x05 // Linearized Temperature High Fault Threshold Register, MSB
#define REG_LTHFTL                  0x06 // Linearized Temperature High Fault Threshold Register, LSB
#define REG_LTLFTH                  0x07 // Linearized Temperature Low Fault Threshold Register, MSB
#define REG_LTLFTL                  0x08 ///< Linearized Temperature Low Fault Threshold Register, LSB
#define REG_CJTO                    0x09  // Cold-Junction Temperature Offset Register
#define REG_CJTH                    0x0A  // Cold-Junction Temperature Register, MSB
#define REG_CJTL                    0x0B  // Cold-Junction Temperature Register, LSB
#define REG_LTCBH                   0x0C  // Linearized TC Temperature, Byte 2
#define REG_LTCBM                   0x0D  // Linearized TC Temperature, Byte 1
#define REG_LTCBL                   0x0E  // Linearized TC Temperature, Byte 0
#define REG_STATUS                  0x0F  // Fault Status Register

#define  FAULT_CJRANGE  0x80 // Fault status Cold Junction Out-of-Range flag
#define  FAULT_TCRANGE  0x40 // Fault status Thermocouple Out-of-Range flag
#define  FAULT_CJHIGH   0x20 // Fault status Cold-Junction High Fault flag
#define  FAULT_CJLOW    0x10 // Fault status Cold-Junction Low Fault flag
#define  FAULT_TCHIGH   0x08 // Fault status Thermocouple Temperature High Fault flag
#define  FAULT_TCLOW    0x04 // Fault status Thermocouple Temperature Low Fault flag
#define  FAULT_OVUV     0x02 // Fault status Overvoltage or Undervoltage Input Fault flag
#define  FAULT_OPEN     0x01 // Fault status Thermocouple Open-Circuit Fault flag

#endif // __MAX31856_REGS_H
