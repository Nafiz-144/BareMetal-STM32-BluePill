#include "stm32f10x.h"

int main(){
    RCC->APB2ENR |= ((1 << 0) | (1 << 2) | (1 << 3));

    GPIOB->CRL &= ~((1 << 23) | (1 << 22) | (1 << 21) | (1 << 20));
    GPIOB->CRL |= (1 << 22);

    GPIOA->CRL &= ~((1 << 19) | (1 << 18) | (1 << 17) | (1 << 16));
    GPIOA->CRL |= ((1 << 17) | (1 << 16));

    AFIO->EXTICR[1] &= ~(0xf << 4);
    AFIO->EXTICR[1] |= (0x1 << 4);

    EXTI->IMR |= (1 << 5);
    EXTI->RTSR |= (1 << 5);

    NVIC_SetPriority(EXTI9_5_IRQn, 1);
    NVIC_EnableIRQ(EXTI9_5_IRQn);

    while(1) {
    }
}

void EXTI9_5_IRQHandler(void) {
    if (EXTI->PR & (1 << 5)) {
        EXTI->PR = (1 << 5);
        GPIOA->ODR ^= (1 << 4);
    }
}
