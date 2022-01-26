#include "stm32f407xx.h"

volatile uint32_t time = 0;

void wait(uint32_t ms)
{
    SysTick->VAL = 0;
    while (ms)
    {
        if (SysTick->CTRL & 1 << 16)
        {
            ms--;
        }
    }
}

int main()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER6_0;

    while (1)
    {
        wait(1000);
        GPIOA->ODR ^= GPIO_ODR_ODR_6;
    }
}
