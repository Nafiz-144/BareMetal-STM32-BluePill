#include "stm32f10x.h"  

void delays(int time);

int main(void) {
    // 1. Enable clock for GPIOA and GPIOC
    RCC->APB2ENR |= (1 << 2);  // Enable PORTA
    RCC->APB2ENR |= (1 << 4);  // Enable PORTC

    // 2. Configure PA0 as input with pull-up
    GPIOA->CRL &= ~(0xF << 0);  // Clear PA0 configuration
    GPIOA->CRL |= (0x8 << 0);   // Set PA0 as input with pull-up/pull-down
    GPIOA->ODR |= (1 << 0);     // Enable pull-up resistor on PA0

    // 3. Configure PC13 as output push-pull
    GPIOC->CRH &= ~(0xF << 20);  // Clear PC13 configuration
    GPIOC->CRH |= (0x2 << 20);   // Set PC13 as output (2MHz, push-pull)

    while (1) {
        if (!(GPIOA->IDR & (1 << 0))) {  // Button pressed (active low)
            delays(10);
            GPIOC->ODR ^= (1 << 13);  // Toggle LED
            delays(10);
        }
    }
}

void delays(int time) {
    int t;
    for (; time > 0; time--) {
        for (t = 0; t < 100000; t++);
    }
}
