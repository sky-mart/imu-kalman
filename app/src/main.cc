#include <stm32f3xx_hal.h>
#include "stm32f3xx_hal_gpio.h"

void softDelay(volatile uint32_t time)
{
    while (time > 0) {
        time--;
    }
}

int main()
{
    constexpr uint16_t led = GPIO_PIN_12;

    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin              = led;
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull             = GPIO_PULLUP;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    while (true) {
        HAL_GPIO_TogglePin(GPIOE, led);
        softDelay(40000);
    }
}
