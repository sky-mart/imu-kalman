#include "console_commands.h"
#include "main.h"
#include "console.h"
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern UART_HandleTypeDef huart2;

uint8_t GYRO_Read(uint8_t address);
uint8_t ACCEL_Read(uint8_t address);

CONSOLE_COMMAND_DEF(hello, "Say hello!");

CONSOLE_COMMAND_DEF(led, "Sets a LED No",
    CONSOLE_INT_ARG_DEF(led, "The LED 1...7"),
    CONSOLE_INT_ARG_DEF(value, "The value 0-1")
);

CONSOLE_COMMAND_DEF(spi, "Read whoami of L3GD20");

CONSOLE_COMMAND_DEF(i2c, "Read CTRL_REG_1_A of LSM303DLHC");

void Console_Write(const char* str)
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
    char str[20];
    const uint8_t whoami = GYRO_Read(0xF);
    snprintf(str, sizeof(str), "whoami=0x%X\n", whoami);
    Console_Write(str);
}

static void i2c_command_handler(const i2c_args_t* args)
{
    char str[20];
    const uint8_t ctrl_reg_1_a = ACCEL_Read(0x20);
    snprintf(str, sizeof(str), "ctrl_reg_1_a=0x%X\n", ctrl_reg_1_a);
    Console_Write(str);
}



#ifdef __cplusplus
}
#endif // __cplusplus
