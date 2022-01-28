#include "stm32f407xx.h"
#include <stdio.h>

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

void transmit(uint8_t *data, uint32_t size)
{
    USART1->CR1 |= USART_CR1_TE;

    for (int i = 0; i < size; i++)
    {
        while (!(USART1->SR & USART_SR_TXE))
        {
        }
        USART1->DR = data[i];
    }
}

int main()
{
    // GPIO and clock for LED2
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER6_0;

    // GPIO and clock for USART1 (PA9 PA10)
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN_Msk;

    GPIOA->MODER |= GPIO_MODER_MODE9_1;
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0 | GPIO_AFRH_AFSEL9_1 | GPIO_AFRH_AFSEL9_2;

    // GPIOA->MODER |= GPIO_MODER_MODE10_1;
    // GPIOA->AFR[1] |= GPIO_AFRH_AFSEL10_0 | GPIO_AFRH_AFSEL10_1 | GPIO_AFRH_AFSEL10_2;

    // configuration for USART1
    USART1->BRR = (uint16_t)(16000000 / 115200);
    USART1->CR1 |= USART_CR1_UE_Msk;

    uint32_t num = 0;
    while (1)
    {
        num++;

        char data[256];
        uint32_t size = 0;
        size = sprintf(data, "dat1\n");
        transmit(data, size);

        wait(1000*1000);
        GPIOA->ODR ^= GPIO_ODR_ODR_6;
    }
}
