#include "l3gd20.h"

namespace mart
{
namespace
{
constexpr uint32_t defaultTimeout = 10;
constexpr uint8_t READ_MASK       = 0x80;
constexpr uint8_t MULTI_MASK = 0x40;
}

L3GD20::L3GD20(GPIO_TypeDef* csPort, uint16_t csPin, SPI_HandleTypeDef& hspi)
    : csPort_(csPort), csPin_(csPin), hspi_(hspi)
{
}

uint8_t L3GD20::read(Register reg)
{
    uint8_t data = static_cast<uint8_t>(reg) | READ_MASK;

    csLow();
    // TODO: handle error codes
    HAL_SPI_Transmit(&hspi_, &data, sizeof(data), defaultTimeout);
    HAL_SPI_Receive(&hspi_, &data, sizeof(data), defaultTimeout);
    csHigh();
    return data;
}

void L3GD20::write(Register reg, uint8_t value)
{
    uint8_t data[2] = {static_cast<uint8_t>(reg), value};

    csLow();
    HAL_SPI_Transmit(&hspi_, data, sizeof(data), defaultTimeout);
    csHigh();
}

void L3GD20::multiRead(Register startReg, uint8_t* data, uint8_t count)
{
    uint8_t reg = static_cast<uint8_t>(startReg) | READ_MASK | MULTI_MASK;

    csLow();
    HAL_SPI_Transmit(&hspi_, &reg, sizeof(reg), defaultTimeout);
    HAL_SPI_Receive(&hspi_, data, count, defaultTimeout);
    csHigh();
}

void L3GD20::multiWrite(Register startReg, uint8_t* data, uint8_t count)
{
    uint8_t reg = static_cast<uint8_t>(startReg) | MULTI_MASK;

    csLow();
    HAL_SPI_Transmit(&hspi_, &reg, sizeof(reg), defaultTimeout);
    HAL_SPI_Transmit(&hspi_, data, sizeof(data), defaultTimeout);
    csHigh();
}

void L3GD20::read(Vector3& angularRates)
{
    uint8_t data[6];
    multiRead(mart::L3GD20::Register::OUT_X_L, data, sizeof(data));

    angularRates.x = static_cast<int16_t>(data[0]) | static_cast<int16_t>(data[1]) << 8;
    angularRates.y = static_cast<int16_t>(data[2]) | static_cast<int16_t>(data[3]) << 8;
    angularRates.z = static_cast<int16_t>(data[4]) | static_cast<int16_t>(data[5]) << 8;
}

void L3GD20::csLow()
{
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_RESET);
}

void L3GD20::csHigh()
{
    HAL_GPIO_WritePin(csPort_, csPin_, GPIO_PIN_SET);
}

}
