#include "stm32f407xx.h"
#include <stdio.h>

void wait(uint32_t ms)
{
    SysTick->VAL = 0;
    while (ms)
    {
        if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
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

void config()
{
    // clock for GPIOA (AHB1)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // GPIO for LED2 (A6)
    GPIOA->MODER |= GPIO_MODER_MODER6_0;

    // clock for USART1 (APB2)
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN_Msk;

    // AF for USART1 (TX PA9; RX PA10)
    GPIOA->MODER |= GPIO_MODER_MODE9_1;
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0 |
                     GPIO_AFRH_AFSEL9_1 |
                     GPIO_AFRH_AFSEL9_2;
    // GPIOA->MODER |= GPIO_MODER_MODE10_1;
    // GPIOA->AFR[1] |= GPIO_AFRH_AFSEL10_0 |
    //                  GPIO_AFRH_AFSEL10_1 |
    //                  GPIO_AFRH_AFSEL10_2;

    // configuration for USART1
    USART1->BRR = (uint16_t)(16000000 / 115200);
    USART1->CR1 |= USART_CR1_UE_Msk;
}

int main()
{
    config();

    while (1)
    {
        char data[256];
        uint32_t size = sprintf(data, "dat1\n");
        transmit(data, size);

        wait(1000 * 1000);
        GPIOA->ODR ^= GPIO_ODR_ODR_6;
    }
}
