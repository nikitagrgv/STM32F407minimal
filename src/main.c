#include "stm32f407xx.h"


void wait(uint32_t count)
{
    volatile uint32_t i = count;
    while (i--)
        ;
}

int main()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER6_0;

    while (1)
    {
        wait(5e5);
        GPIOA->ODR ^= GPIO_ODR_ODR_6;
    }
}
