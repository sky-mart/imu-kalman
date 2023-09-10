#ifndef L3GD20_H
#define L3GD20_H

#include "types.h"
#include "stm32f3xx_hal.h"

namespace mart
{

class L3GD20
{
public:
    enum class Register : uint8_t {
        WHO_AM_I      = 0xF,
        CTRL_REG1     = 0x20,
        CTRL_REG2     = 0x21,
        CTRL_REG3     = 0x22,
        CTRL_REG4     = 0x23,
        CTRL_REG5     = 0x24,
        REFERENCE     = 0x25,
        OUT_TEMP      = 0x26,
        STATUS_REG    = 0x27,
        OUT_X_L       = 0x28,
        OUT_X_H       = 0x29,
        OUT_Y_L       = 0x2A,
        OUT_Y_H       = 0x2B,
        OUT_Z_L       = 0x2C,
        OUT_Z_H       = 0x2D,
        FIFO_CTRL_REG = 0x2E,
        FIFO_SRC_REG  = 0x2F,
        INT1_CFG      = 0x30,
        INT1_SRC      = 0x31,
        INT1_TSH_XH   = 0x32,
        INT1_TSH_XL   = 0x33,
        INT1_TSH_YH   = 0x34,
        INT1_TSH_YL   = 0x35,
        INT1_TSH_ZH   = 0x36,
        INT1_TSH_ZL   = 0x37,
        INT1_DURATION = 0x38
    };

    L3GD20(GPIO_TypeDef* csPort, uint16_t csPin, SPI_HandleTypeDef& hspi);

    uint8_t read(Register reg);
    void write(Register reg, uint8_t value);

    void multiRead(Register startReg, uint8_t* data, uint8_t count);
    void multiWrite(Register startReg, uint8_t* data, uint8_t count);

    void read(Vector3& angularRates);

private:
    void csLow();
    void csHigh();

    GPIO_TypeDef* const csPort_;
    const uint16_t csPin_;
    SPI_HandleTypeDef& hspi_;
};

}  // namespace mart

#endif /* L3GD20_H */
