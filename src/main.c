#include "stm32f407xx.h"
#include <stdio.h>

#define OW_HIGH() (GPIOA->ODR |= GPIO_ODR_OD1_Msk)
#define OW_LOW() (GPIOA->ODR &= ~GPIO_ODR_OD1_Msk)
#define OW_CHECK() (!!(GPIOA->IDR & GPIO_IDR_ID1_Msk))

void wait(uint32_t us)
{
    SysTick->VAL = 0;
    while (us)
    {
        if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
        {
            us--;
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

    // A1 output open drain, pull up, high
    GPIOA->MODER |= GPIO_MODER_MODE1_0;
    GPIOA->OTYPER |= GPIO_OTYPER_OT1_Msk;
    GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_0;
    GPIOA->ODR |= GPIO_ODR_OD1_Msk;
}

uint8_t DSInit()
{
    OW_LOW();
    wait(550);
    OW_HIGH();
    wait(15);
    for (int i = 0; i < 500; i += 10)
    {
        wait(10);
        if (OW_CHECK() == 0)
        {
            return 1;
        }
    }
    return 0;
}

inline void OW_WriteBit(uint8_t bit)
{
    if (bit == 0)
    {
        OW_LOW();
        wait(100);
        OW_HIGH();
        wait(8);
    }
    else
    {
        OW_LOW();
        wait(8);
        OW_HIGH();
        wait(100);
    }
}

inline uint8_t OW_ReadBit()
{
    OW_LOW();
    wait(8);
    OW_HIGH();
    wait(30);
    uint8_t bit = OW_CHECK();
    wait(70);
    return bit;
}

void OW_WriteByte(uint8_t byte)
{
    for (int i = 0; i < 8; i++)
    {
        OW_WriteBit((byte << i) & 0x1);
    }
}

uint8_t OW_ReadByte()
{
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++)
    {
        byte |= OW_ReadBit() << i;
    }
    return byte;
}

int main()
{
    config();

    OW_HIGH();
    wait(100 * 1000);
    while (1)
    {
        uint8_t is_there = DSInit();
        OW_WriteByte(0x33);
        // OW_WriteByte(0xBE);
        // wait(30);
        uint8_t bytes[9];
        for (int i = 0; i < 9; i++)
        {
            bytes[i] = OW_ReadByte();
        }

        char data[256];
        uint32_t size = sprintf(data, "pad: %X %X %X %X %X %X %X %X %X\n",
                                bytes[0],
                                bytes[1],
                                bytes[2],
                                bytes[3],
                                bytes[4],
                                bytes[5],
                                bytes[6],
                                bytes[7],
                                bytes[8],
                                bytes[9]);
        transmit(data, size);

        wait(1000 * 1000);
        GPIOA->ODR ^= GPIO_ODR_ODR_6;
    }
}
