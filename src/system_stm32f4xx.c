#include "stm32f4xx.h"

void SystemInit(void)
{
    /* FPU settings ------------------------------------------------------------*/
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */

    ClockInit();
    SysTickInit();
}

void ClockInit()
{
    // setting PLL
    // PLLP = 2 by default
    // PLLN = 168
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLN_Msk;
    RCC->PLLCFGR |= 168UL << RCC_PLLCFGR_PLLN_Pos;
    // PLLM = 8
    RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLM_Msk;
    RCC->PLLCFGR |= 8UL << RCC_PLLCFGR_PLLM_Pos;

    // PLL turn on, waiting
    RCC->CR |= RCC_CR_PLLON_Msk;
    while (RCC->CR & RCC_CR_PLLRDY_Msk)
    {
    }

    // FLASH latency 5WS
    FLASH->ACR |= FLASH_ACR_LATENCY_5WS;

    // clock prescalers
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;

    // switch to PLL, waiting
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while (RCC->CFGR & RCC_CFGR_SWS_PLL)
    {
    }
}

void SysTickInit()
{
    // SysTick for 1us
    SysTick->LOAD = 168e6 / 1e6 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
}