#include "stm32f10x.h"                  // Device header

int main(void)
{
	RCC->APB2ENR |=1<<2;
	GPIOA->CRL &= 0XFF0FFFFF;
	GPIOA->CRL |=0X00200000;
	
	
	
	SysTick->LOAD = 8000000 - 1; // 1-second delay for 8 MHz clock
    SysTick->VAL = 0; // Clear counter
    SysTick->CTRL = (1<<2) |(1<<0);

    while (1)
    {
        if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) {
            GPIOA->ODR ^= GPIO_ODR_ODR5; // Toggle LED on PA5
        }
    }

  

}
