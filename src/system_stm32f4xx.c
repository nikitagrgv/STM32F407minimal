#include "stm32f4xx.h"

void SystemInit(void)
{
    /* FPU settings ------------------------------------------------------------*/
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2)); /* set CP10 and CP11 Full Access */

    // SysTick for 1us
    SysTick->LOAD = 16000 / 1000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
}
