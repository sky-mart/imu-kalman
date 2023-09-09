#include "lsm303dlhc.h"

namespace mart
{

namespace
{
constexpr uint32_t DEFAULT_TIMEOUT_MS =
    10;  // TODO: make sure it's measured in ms
constexpr uint8_t ACCELEROMETER_I2C_ADDRESS = 0x32;
constexpr uint8_t MAGNETOMETER_I2C_ADDRESS  = 0x3C;
constexpr uint8_t READ_MASK                 = 0x1;
}  // namespace

Lsm303dlhc::Lsm303dlhc(I2C_HandleTypeDef& hi2c) : hi2c_(hi2c) {}

void Lsm303dlhc::read(uint8_t devAddr,
                      uint8_t regAddr,
                      uint8_t* data,
                      uint8_t count)
{
    // TODO: handle error codes
    HAL_I2C_Mem_Read(&hi2c_, static_cast<uint16_t>(devAddr | READ_MASK),
                     static_cast<uint16_t>(regAddr), sizeof(regAddr), data,
                     count, DEFAULT_TIMEOUT_MS);
}

void Lsm303dlhc::write(uint8_t devAddr,
                       uint8_t regAddr,
                       uint8_t* data,
                       uint8_t count)
{
    HAL_I2C_Mem_Write(&hi2c_, static_cast<uint16_t>(devAddr),
                      static_cast<uint16_t>(regAddr), sizeof(regAddr), data,
                      count, DEFAULT_TIMEOUT_MS);
}

uint8_t Lsm303dlhc::read(AccRegister reg)
{
    uint8_t value;
    read(ACCELEROMETER_I2C_ADDRESS, static_cast<uint8_t>(reg), &value,
         sizeof(value));
    return value;
}

void Lsm303dlhc::write(AccRegister reg, uint8_t value)
{
    write(ACCELEROMETER_I2C_ADDRESS, static_cast<uint8_t>(reg), &value,
          sizeof(value));
}

void Lsm303dlhc::read(AccRegister startReg, uint8_t* data, uint8_t count)
{
    read(ACCELEROMETER_I2C_ADDRESS, static_cast<uint8_t>(startReg), data,
         count);
}

void Lsm303dlhc::write(AccRegister startReg, uint8_t* data, uint8_t count)
{
    write(ACCELEROMETER_I2C_ADDRESS, static_cast<uint8_t>(startReg), data,
          count);
}

uint8_t Lsm303dlhc::read(MagRegister reg)
{
    uint8_t value;
    read(MAGNETOMETER_I2C_ADDRESS, static_cast<uint8_t>(reg), &value,
         sizeof(value));
    return value;
}

void Lsm303dlhc::write(MagRegister reg, uint8_t value)
{
    write(MAGNETOMETER_I2C_ADDRESS, static_cast<uint8_t>(reg), &value,
          sizeof(value));
}

void Lsm303dlhc::read(MagRegister startReg, uint8_t* data, uint8_t count)
{
    read(MAGNETOMETER_I2C_ADDRESS, static_cast<uint8_t>(startReg), data, count);
}

void Lsm303dlhc::write(MagRegister startReg, uint8_t* data, uint8_t count)
{
    write(MAGNETOMETER_I2C_ADDRESS, static_cast<uint8_t>(startReg), data,
          count);
}

void Lsm303dlhc::readAcceleration(Vector3& accelerations)
{
    uint8_t data[6];
    data[0] = read(AccRegister::OUT_X_L_A);
    data[1] = read(AccRegister::OUT_X_H_A);
    data[2] = read(AccRegister::OUT_Y_L_A);
    data[3] = read(AccRegister::OUT_Y_H_A);
    data[4] = read(AccRegister::OUT_Z_L_A);
    data[5] = read(AccRegister::OUT_Z_H_A);
    // TODO: might be way faster
    // also, think about MSB first configuration
    // read(AccRegister::OUT_X_L_A, data, sizeof(data));

    accelerations.x = static_cast<int16_t>(data[0]) | static_cast<int16_t>(data[1]) << 8;
    accelerations.y = static_cast<int16_t>(data[2]) | static_cast<int16_t>(data[3]) << 8;
    accelerations.z = static_cast<int16_t>(data[4]) | static_cast<int16_t>(data[5]) << 8;
}

void Lsm303dlhc::readMagneticField(Vector3& magneticField)
{
    uint8_t data[6];
    data[0] = read(MagRegister::OUT_X_H_M);
    data[1] = read(MagRegister::OUT_X_L_M);
    data[2] = read(MagRegister::OUT_Y_H_M);
    data[3] = read(MagRegister::OUT_Y_L_M);
    data[4] = read(MagRegister::OUT_Z_H_M);
    data[5] = read(MagRegister::OUT_Z_L_M);
    // read(MagRegister::OUT_X_H_M, data, sizeof(data));

    magneticField.x = static_cast<int16_t>(data[1]) | static_cast<int16_t>(data[0]) << 8;
    magneticField.y = static_cast<int16_t>(data[3]) | static_cast<int16_t>(data[2]) << 8;
    magneticField.z = static_cast<int16_t>(data[5]) | static_cast<int16_t>(data[4]) << 8;
}

}  // namespace mart
