#include <stm32f3xx.h>

void softDelay(volatile uint32_t time)
{
    while (time > 0) {
        time--;
    }
}

int main()
{
    constexpr unsigned int led = 8;

    RCC->AHBENR |= RCC_AHBENR_GPIOEEN;  // Turn tacting GPIOE on

    GPIOE->MODER |= 1 << led * 2;
    GPIOE->OTYPER &= ~(1 << led);
    GPIOE->OSPEEDR |= 3 << led * 2;
    GPIOE->PUPDR |= 1 << led * 2;

    while (true) {
        GPIOE->BSRR = 1 << led;
        softDelay(400000);
        GPIOE->BSRR = 1 << (led + 16);
        softDelay(400000);
    }
}
