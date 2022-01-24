#include "stm32f407xx.h"

int main()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER6_0;

    while (1)
    {
        GPIOA->ODR |= GPIO_ODR_ODR_6;
        volatile int i = 5e5;
        while (i--)
        {
        }

        GPIOA->ODR &= ~GPIO_ODR_ODR_6;

        i = 5e5;
        while (i--)
        {
        }
    }
}
