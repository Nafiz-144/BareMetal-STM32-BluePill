#include "stm32f10x.h" // Device header

void Delays(int time);

int main(void) {
    // Enable clocks for GPIOA and GPIOC
    RCC->APB2ENR |= (1 << 2);  // Enable clock for GPIOA
    RCC->APB2ENR |= (1 << 4);  // Enable clock for GPIOC

    // Configure GPIOA PIN 0 as Input (Floating Input)
    GPIOA->CRL &= ~(0xF << 0); // Clear configuration for PIN 0
    GPIOA->CRL |= (0x4 << 0);  // Set PIN 0 as Floating Input

    // Configure GPIOC PIN 13 as Output (Push-Pull)
    GPIOC->CRH &= ~(0xF << 20); // Clear configuration for PIN 13
    GPIOC->CRH |= (0x2 << 20);  // Set PIN 13 as General Purpose Output, 2 MHz

    while (1) {
        if (GPIOA->IDR & (1 << 0)) {  // Check if PIN A0 is HIGH
            Delays(10);               // Debounce delay
            GPIOC->ODR ^= (1 << 13);  // Toggle PIN C13
            Delays(10);               // Delay for visible blinking
        } else {
            GPIOC->ODR |= (1 << 13);  // Ensure PIN C13 is HIGH
        }
    }
}

void Delays(int time) {
    volatile int t;
    for (; time > 0; time--) {
        for (t = 0; t < 100000; t++) {
            // Waste time for delay
        }
    }
}
