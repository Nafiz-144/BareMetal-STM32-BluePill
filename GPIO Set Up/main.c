#include "stm32f10x.h"

void pinA();
void pinB();
void pinC();
void delay(uint32_t ms);

int main() {
    pinA();
    pinB();
    pinC();
    while (1) {
        GPIOA->ODR ^= (1<<12);
        delay(500);
        GPIOC->ODR ^= (1<<14);
        delay(100);
        GPIOB->ODR ^= (1<<3);
        delay(200);
    }
}

void pinA() {
    RCC->APB2ENR |= (1 << 2);
    GPIOA->CRH &= ~((1<<19) | (1<<18) | (1<<16));
    GPIOA->CRH |= (1<<17);
}

void pinB() {
    RCC->APB2ENR |= (1 << 3);
    GPIOB->CRL &= ~((1<<13) | (1<<12) | (1<<10));
    GPIOB->CRL |= (1<<11);
}

void pinC() {
    RCC->APB2ENR |= (1 << 4);
    GPIOC->CRH &= ~((1<<27) | (1<<26) | (1<<24));
    GPIOC->CRH |= (1<<25);
}

void delay() {
    for (int i = 0; i < 1000000; i++);
}