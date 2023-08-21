#include <stm32f3xx_hal.h>

extern "C" {
void SysTick_Handler(void) {
    HAL_IncTick();
}
}

int main()
{
    constexpr uint16_t led = GPIO_PIN_10;

    HAL_Init();

    __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin              = led;
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull             = GPIO_PULLUP;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    while (true) {
        HAL_GPIO_TogglePin(GPIOE, led);
        HAL_Delay(100);
    }
}
