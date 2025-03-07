#include "stm32f10x.h"    // Device header for STM32F103C8

void systickDelayMs(int n);

int main(void)
{
    // Enable the GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    
    // Configure PA5 as output
    GPIOA->CRL &= ~(0xF << 20); // Clear mode and configuration bits for PA5
    GPIOA->CRL |= (0x1 << 20);  // Set PA5 as output push-pull, max speed 10 MHz

    while(1)
    {
        systickDelayMs(300);
        GPIOA->ODR ^= GPIO_ODR_ODR5; // Toggle PA5
    }
}

void systickDelayMs(int n)
{
    SysTick->LOAD = 72000; // Reload with number of clocks per millisecond (72MHz / 1000)
    SysTick->VAL = 0;      // Clear current value register
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk; // Enable Systick with processor clock

    for(int i = 0; i < n; i++)
    {
        // Wait until the COUNTFLAG is set
        while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0)
        {
        }
    }

    SysTick->CTRL = 0; // Disable Systick
}