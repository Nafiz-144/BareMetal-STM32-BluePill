#include "stm32f10x.h"
#include <stdio.h>

void USART1_Init(void);
void USART1_SendChar(char c);
void USART1_SendString(char *str);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
uint32_t read_distance_cm(void);

void GPIO_Init(void) {
    // Enable GPIOA and USART1 clocks
    RCC->APB2ENR |= (1 << 2) | (1 << 14);

    // PA0 = TRIG (output), PA1 = ECHO (input)
    GPIOA->CRL &= ~((0xF << 0) | (0xF << 4));      // Clear CNF/MODE for PA0, PA1
    GPIOA->CRL |= (0x2 << 0);                      // PA0: Output push-pull, 2 MHz
    GPIOA->CRL |= (0x4 << 4);                      // PA1: Input floating

    // PA9 = USART1_TX
    GPIOA->CRH &= ~(0xF << 4);                     // Clear PA9
    GPIOA->CRH |= (0xB << 4);                      // Alt push-pull, 50 MHz
}

int main(void) {
    SystemInit();
    GPIO_Init();
    USART1_Init();

    USART1_SendString("HC-SR04 Initialized...\r\n");

    while (1) {
        uint32_t distance = read_distance_cm();
        char buffer[50];
        sprintf(buffer, "Distance: %lu cm\r\n", distance);
        USART1_SendString(buffer);
        delay_ms(500);
    }
}

uint32_t read_distance_cm(void) {
    // Send 10us pulse on TRIG
    GPIOA->BSRR = (1 << 0);         // Set PA0 HIGH
    delay_us(10);
    GPIOA->BRR = (1 << 0);          // Set PA0 LOW

    // Wait for ECHO to go HIGH
    while (!(GPIOA->IDR & (1 << 1)));

    // Measure high time
    uint32_t start = 0, end = 0;
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL = 0;
    SysTick->CTRL = 5; // Enable SysTick, no interrupt, processor clock

    start = SysTick->VAL;
    while (GPIOA->IDR & (1 << 1));  // Wait while ECHO is high
    end = SysTick->VAL;

    SysTick->CTRL = 0; // Stop SysTick

    uint32_t time_us = (start > end) ? (start - end) : (0xFFFFFF - end + start);
    time_us = time_us / 8; // 72MHz -> 1 tick = ~0.014 us

    uint32_t distance_cm = time_us / 58;
    return distance_cm;
}

void delay_us(uint32_t us) {
    for (uint32_t i = 0; i < us * 8; i++) {
        __NOP();
    }
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        delay_us(1000);
    }
}

// USART1 TX on PA9
void USART1_Init(void) {
    USART1->BRR = 0x1D4C;  // 72MHz / 9600 baud
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE;
}

void USART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

void USART1_SendString(char *str) {
    while (*str) {
        USART1_SendChar(*str++);
    }
}
