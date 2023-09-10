#include "console_commands.h"
#include "main.h"
#include "console.h"
#include "l3gd20.h"
#include "lsm303dlhc.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <cstddef>

extern mart::L3GD20 gyroscope;
extern mart::Lsm303dlhc lsm303dlhc;
extern UART_HandleTypeDef huart2;

CONSOLE_COMMAND_DEF(hello, "Say hello!");

CONSOLE_COMMAND_DEF(led, "Sets a LED No",
    CONSOLE_INT_ARG_DEF(led, "The LED 1...7"),
    CONSOLE_INT_ARG_DEF(value, "The value 0-1")
);

CONSOLE_COMMAND_DEF(spi, "Read whoami of L3GD20");

CONSOLE_COMMAND_DEF(i2c, "Read CTRL_REG_1_A of LSM303DLHC");


static void Console_Write(const char* str)
{
    const size_t len = strlen(str);
    HAL_UART_Transmit(&huart2, (const uint8_t*)str, len, HAL_MAX_DELAY);
}

void Console_Init()
{
    const console_init_t init_console = {
        .write_function = Console_Write,
    };

    console_init(&init_console);

    console_command_register(hello);
    console_command_register(led);
    console_command_register(spi);
    console_command_register(i2c);
}

void Console_Printf(const char* format, ...)
{
    static char str[100];
    va_list args;

    va_start(args, format);
    vsnprintf(str, sizeof(str), format, args);
    va_end(args);

    Console_Write(str);
}

static void hello_command_handler(const hello_args_t* args)
{
    Console_Write("Hello!\n");
}

static void led_command_handler(const led_args_t* args)
{
    const uint8_t led = args->led;
    const uint8_t value = args->value;

    if (led < 1 || led > 7 || value > 1)
    {
        Console_Write("Wrong argument\n");
        return;
    }

    switch(led)
    {
    case 1: HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, (GPIO_PinState)value); break;
    case 2: HAL_GPIO_WritePin(LD5_GPIO_Port, LD5_Pin, (GPIO_PinState)value); break;
    case 3: HAL_GPIO_WritePin(LD7_GPIO_Port, LD7_Pin, (GPIO_PinState)value); break;
    case 4: HAL_GPIO_WritePin(LD9_GPIO_Port, LD9_Pin, (GPIO_PinState)value); break;
    case 5: HAL_GPIO_WritePin(LD10_GPIO_Port, LD10_Pin, (GPIO_PinState)value); break;
    case 6: HAL_GPIO_WritePin(LD8_GPIO_Port, LD8_Pin, (GPIO_PinState)value); break;
    case 7: HAL_GPIO_WritePin(LD6_GPIO_Port, LD6_Pin, (GPIO_PinState)value); break;
    default: break;
    }
}

static void spi_command_handler(const spi_args_t* args)
{
    mart::L3GD20::AngularRates rates;
    gyroscope.read(rates);
    Console_Printf("x=%d, y=%d, z=%d\n", rates.x, rates.y, rates.z);
}

static void i2c_command_handler(const i2c_args_t* args)
{
    lsm303dlhc.write(mart::Lsm303dlhc::AccRegister::CTRL_REG1_A, 0x57);
    const uint8_t ctrl_reg1_a = lsm303dlhc.read(mart::Lsm303dlhc::AccRegister::CTRL_REG1_A);
    Console_Printf("ctrl_reg1_a=0x%X\n", ctrl_reg1_a);

    lsm303dlhc.write(mart::Lsm303dlhc::MagRegister::CRA_REG_M, 0x18);
    const uint8_t cra_reg_m = lsm303dlhc.read(mart::Lsm303dlhc::MagRegister::CRA_REG_M);
    Console_Printf("cra_reg_m=0x%X\n", cra_reg_m);
}
