#include "stm32f10x.h"                  // Device header
/*
// GPIO initialization function
void gpio_init(void) {
    RCC->APB2ENR |= (1 << 2);  // Enable clock for GPIOA (APB2)

    // Configure PA5 as output push-pull (LED)
    GPIOA->CRL &= ~((1 << 23) | (1 << 22) | (1 << 21) | (1 << 20));  // Reset PA5
    GPIOA->CRL |= ((1 << 23) | (1 << 21) | (1 << 20));  // Set PA5 as output push-pull (50 MHz)

    // Configure PA0 as output push-pull (TRIG for HC-SR04)
    GPIOA->CRL &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0));  // Reset PA0
    GPIOA->CRL |= ((1 << 3) | (1 << 1));  // Set PA0 as output (2 MHz)

    // Configure PA1 as input floating (ECHO for HC-SR04)
    GPIOA->CRL &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4));  // Reset PA1
    GPIOA->CRL |= (1 << 6);  // Set PA1 as input floating
}

// Timer initialization function for TIM2
void tim_init(void) {
    RCC->APB1ENR |= (1 << 0);  // Enable clock for TIM2
    TIM2->PSC = 71 - 1;  // Prescaler to get 1 µs timer resolution
    TIM2->ARR = 0xFFFF;  // Set auto-reload register to maximum
    TIM2->CNT = 0;  // Reset the counter
    TIM2->CR1 |= (1 << 0);  // Enable TIM2
}

// Delay function using TIM2 for microseconds
void delay_us_tim2(uint16_t US) {
    TIM2->CNT = 0;  // Reset timer counter
    while (TIM2->CNT < US);  // Wait until the counter reaches the desired delay
}

// Function to read distance from the HC-SR04
uint32_t read_distance_cm(void) {
    GPIOA->BSRR = (1 << 0);  // Set PA0 (TRIG) HIGH
    delay_us_tim2(10);  // 10 µs delay
    GPIOA->BRR = (1 << 0);  // Set PA0 (TRIG) LOW

    // Wait for ECHO pin (PA1) to go HIGH
    while (!(GPIOA->IDR & (1 << 1)));  // Wait for ECHO HIGH
    TIM2->CNT = 0;  // Reset timer
    // Wait while ECHO pin (PA1) is HIGH
    while (GPIOA->IDR & (1 << 1));  // Wait while ECHO is HIGH

    uint32_t time_us = TIM2->CNT;  // Read the timer counter value
    return time_us / 58;  // Return the distance in centimeters (assuming sound speed 340 m/s)
}

int main(void) {
    gpio_init();  // Initialize GPIO
    tim_init();   // Initialize TIM2 for microsecond delays

    while (1) {
        uint32_t distance = read_distance_cm();  // Get the distance from HC-SR04

        if (distance <= 2) {
            GPIOA->BSRR = (1 << 5);  // Turn ON LED at PA5 if distance <= 5 cm
        } else {
            GPIOA->BRR = (1 << 5);  // Turn OFF LED if distance > 5 cm
        }

        delay_us_tim2(60000);  // ~60ms delay to avoid rapid flickering of the LED
    }
}*/


void gpio_init(void) {
    RCC->APB2ENR |= (1 << 2);  // Enable clock for GPIOA (APB2)

    // Configure PA5 as output push-pull (50 MHz)
    GPIOA->CRL &= ~((1 << 23) | (1 << 22) | (1 << 21) | (1 << 20));  // Reset PA5
    GPIOA->CRL |= ((1 << 23) | (1 << 21) | (1 << 20));  // Set PA5 as output (50 MHz)
}

void delay_ms(uint32_t ms) {
    // Simple delay loop (assuming a 72 MHz clock, this gives ~1ms delay per iteration)
    for (uint32_t i = 0; i < ms * 1000; i++) {
        __NOP();  // No operation (dummy instruction to waste time)
    }
}

int main(void) {
    gpio_init();  // Initialize PA5 as output

    while (1) {
        GPIOA->BSRR = (1 << 5);  // Turn ON LED (PA5)
        delay_ms(500);            // Wait 500 ms

        GPIOA->BRR = (1 << 5);   // Turn OFF LED (PA5)
        delay_ms(500);            // Wait 500 ms
    }
}