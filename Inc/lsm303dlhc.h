#ifndef LSM303DLHC_H
#define LSM303DLHC_H

#include "types.h"
#include "stm32f3xx_hal.h"

namespace mart
{

class Lsm303dlhc
{
public:
    enum class AccRegister : uint8_t {
        CTRL_REG1_A = 0x20,
        CTRL_REG2_A = 0x21,
        CTRL_REG3_A = 0x22,
        CTRL_REG4_A = 0x23,
        CTRL_REG5_A = 0x24,
        CTRL_REG6_A = 0x25,
        REFERENCE_A = 0x26,
        STATUS_REG_A = 0x27,
        OUT_X_L_A = 0x28,
        OUT_X_H_A = 0x29,
        OUT_Y_L_A = 0x2A,
        OUT_Y_H_A = 0x2B,
        OUT_Z_L_A = 0x2C,
        OUT_Z_H_A = 0x2D,
        FIFO_CTRL_REG_A = 0x2E,
        FIFO_SRC_REG_A = 0x2F,
        INT1_CFG_A = 0x30,
        INT1_SOURCE_A = 0x31,
        INT1_THS_A = 0x32,
        INT1_DURATION = 0x33,
        INT2_CFG_A = 0x34,
        INT2_SOURCE_A = 0x35,
        INT2_THS_A = 0x36,
        INT2_DURATION = 0x37,
        CLICK_CFG_A = 0x38,
        CLICK_SRC_A = 0x39,
        CLICK_THS_A = 0x3A,
        TIME_LIMIT_A = 0x3B,
        TIME_LATENCY_A = 0x3C,
        TIME_WINDOW_A = 0X3D
    };

    enum class MagRegister : uint8_t {
        CRA_REG_M = 0x00,
        CRB_REG_M = 0x01,
        MR_REG_M = 0x02,
        OUT_X_H_M = 0x03,
        OUT_X_L_M = 0x04,
        OUT_Y_H_M = 0x05,
        OUT_Y_L_M = 0x06,
        OUT_Z_H_M = 0x07,
        OUT_Z_L_M = 0x08,
        SR_REG_Mg = 0x09,
        IRA_REG_M = 0x0A,
        IRB_REG_M = 0x0B,
        IRC_REG_M = 0x0C,
        TEMP_OUT_H_M = 0x31,
        TEMP_OUT_L_M = 0x32
    };

    explicit Lsm303dlhc(I2C_HandleTypeDef& hi2c);

    uint8_t read(AccRegister reg);
    void read(AccRegister startReg, uint8_t* data, uint8_t count);
    void write(AccRegister reg, uint8_t value);
    void write(AccRegister startReg, uint8_t* data, uint8_t count);

    uint8_t read(MagRegister reg);
    void write(MagRegister reg, uint8_t value);
    void read(MagRegister startReg, uint8_t* data, uint8_t count);
    void write(MagRegister startReg, uint8_t* data, uint8_t count);

    void readAcceleration(Vector3& accelerations);
    void readMagneticField(Vector3& magneticField);

private:
    void read(uint8_t devAddr, uint8_t regAddr, uint8_t* data, uint8_t count);
    void write(uint8_t devAddr, uint8_t regAddr, uint8_t* data, uint8_t count);

    I2C_HandleTypeDef& hi2c_;
};

}

#endif /* LSM303DLHC_H */
