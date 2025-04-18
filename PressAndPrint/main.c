#include "stm32f10x.h"  // Device header
#include<stdio.h>

void delay(volatile uint32_t time);
void UART1_SendChar(char c);
void UART1_SendString(char *str);

int main(void) {
    int count = 0;
    uint8_t prev_state = 1;

    // ------- PA0 (Button Input with Pull-up) -------
    RCC->APB2ENR |= (1 << 2); // Enable GPIOA clock
    GPIOA->CRL &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0)); // Clear CNF0 + MODE0
    GPIOA->CRL |= (1 << 2); // Set CNF0[1:0] = 10: Input with pull-up/down
    GPIOA->ODR |= (1 << 0); // Pull-up enabled (set PA0 high)

    // ------- USART1 Initialization (TX on PA9) -------
    RCC->APB2ENR |= (1 << 14); // Enable USART1 clock
    RCC->APB2ENR |= (1 << 0);  // Enable AFIO clock

    // Configure PA9 (USART1_TX) as Alternate Function Push-Pull output
    GPIOA->CRH &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4)); // Clear CNF9 + MODE9
    GPIOA->CRH |= (1 << 6) | (1 << 4); // MODE9 = 10 (Output 2MHz), CNF9 = 10 (AF Push-Pull)

    USART1->BRR = 0x1D4C; // Baud rate = 9600 @ 72MHz
    USART1->CR1 |= (1 << 3);  // Enable Transmitter
    USART1->CR1 |= (1 << 13); // Enable USART

    // ------- Main Loop -------
    while (1) {
        uint8_t state = (GPIOA->IDR & (1 << 0)) ? 1 : 0; // Read PA0

        if (state == 0 && prev_state == 1) { // Detect falling edge
            count++;

            char msg[30];
            sprintf(msg, "Pressed: %d times\r\n", count);
            UART1_SendString(msg);

            delay(100000); // Crude debounce
        }

        prev_state = state;
    }
}

// Simple delay function
void delay(volatile uint32_t time) {
    while (time--);
}

// Send a single character over USART1
void UART1_SendChar(char c) {
    while (!(USART1->SR & (1 << 7))); // Wait until TXE is set
    USART1->DR = c;
}

// Send a string over USART1
void UART1_SendString(char *str) {
    while (*str) {
        UART1_SendChar(*str++);
    }
}
