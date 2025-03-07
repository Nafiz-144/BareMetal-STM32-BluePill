#include "stm32f10x.h"

int main(void)
{
    RCC->APB2ENR |= (1 << 2);
    GPIOA->CRL &= ~(0xF << 20);
    GPIOA->CRL |= (0xB << 20);
    RCC->APB1ENR |= (1 << 0);
    TIM2->PSC = 7200 - 1;
    TIM2->ARR = 10000 - 1;
    TIM2->CCMR1 |= (0x3 << 4);
    TIM2->CCR1 = 0;
    TIM2->CCER |= (1 << 0);
    TIM2->CNT = 0;
    TIM2->CR1 |= (1 << 0);

    while (1)
    {
    }
}