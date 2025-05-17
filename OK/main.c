#include "stm32f10x.h"  // Device header

// GPIO initialization function
void gpio_init(void) {
    RCC->APB2ENR |= (1 << 2);  // Enable clock for GPIOA

    // PA5 as output push-pull (LED, 2 MHz)
    GPIOA->CRL &= ~((1 << 23) | (1 << 22) | (1 << 21) | (1 << 20));
    GPIOA->CRL |= (1 << 20);  // MODE = 10 (2 MHz), CNF = 00 (push-pull)

    // PA0 as output push-pull (TRIG)
    GPIOA->CRL &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0));
    GPIOA->CRL |= (1 << 1);  // MODE = 10 (2 MHz), CNF = 00

    // PA1 as input floating (ECHO)
    GPIOA->CRL &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4));
    GPIOA->CRL |= (1 << 6);  // MODE = 00, CNF = 01 (floating input)
}

// Timer2 initialization for 1 µs resolution
void tim_init(void) {
    RCC->APB1ENR |= (1 << 0);  // Enable TIM2
    TIM2->PSC = 71;            // Prescaler: 72 MHz / (71 + 1) = 1 MHz (1 µs)
    TIM2->ARR = 0xFFFF;        // Max auto-reload
    TIM2->CNT = 0;             // Reset counter
    TIM2->CR1 |= (1 << 0);     // Enable TIM2
}

// Delay in microseconds using TIM2
void delay_us_tim2(uint16_t us) {
    TIM2->CNT = 0;
    while (TIM2->CNT < us);
}

// Measure distance in cm with timeout protection
uint32_t read_distance_cm(void) {
    // Trigger pulse
    GPIOA->BSRR = (1 << 0);      // TRIG HIGH
    delay_us_tim2(10);           // 10 µs pulse
    GPIOA->BRR = (1 << 0);       // TRIG LOW

    // Wait for ECHO to go HIGH with timeout
    uint32_t timeout = 30000;
    while (!(GPIOA->IDR & (1 << 1)) && --timeout);
    if (timeout == 0) return 999;  // No echo received

    TIM2->CNT = 0;  // Start timing

    // Wait while ECHO stays HIGH with timeout
    timeout = 30000;
    while ((GPIOA->IDR & (1 << 1)) && --timeout);
    if (timeout == 0) return 999;

    uint32_t time_us = TIM2->CNT;
    return time_us / 58;  // Convert to cm
}

int main(void) {
    gpio_init();  // Init GPIO
    tim_init();   // Init Timer2

    while (1) {
        uint32_t distance = read_distance_cm();  // Read distance in cm

        if (distance <= 5) {
            GPIOA->BSRR = (1 << 5);  // LED ON
        } else {
            GPIOA->BRR = (1 << 5);   // LED OFF
        }

        delay_us_tim2(60000);  // ~60 ms delay
    }
}
