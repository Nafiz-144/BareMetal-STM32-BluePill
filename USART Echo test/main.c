#include "stm32f10x.h"                  // Device header
  // Core header for STM32F103

// Function prototypes
void USART2_Init(void);
void USART2_SendChar(char ch);
void DelayMs(uint32_t ms);

int main(void) {
    USART2_Init();  // Initialize USART2
    
    while (1) {
        USART2_SendChar('Y');
        USART2_SendChar('i');
        DelayMs(100);  // Delay 100ms between transmissions
    }
}

// Initialize USART2 (PA2-TX, 9600 baud @ 8MHz PCLK1)
void USART2_Init(void) {
    // 1. Enable clocks for GPIOA and USART2
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;    // Enable GPIOA clock (APB2)
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  // Enable USART2 clock (APB1)
    
    // 2. Configure PA2 (TX) as alternate function push-pull (50MHz)
    GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2);  // Clear bits
    GPIOA->CRL |= GPIO_CRL_MODE2;          // Output mode, max speed (11)
    GPIOA->CRL |= GPIO_CRL_CNF2_1;         // Alternate function push-pull (10)
    
    // 3. Configure USART2 (9600 baud, 8N1, Tx enabled)
    USART2->BRR = 8000000 / 9600;          // PCLK1 = 8MHz (HSI default / 2)
    USART2->CR1 |= USART_CR1_TE;           // Enable transmitter
    USART2->CR1 |= USART_CR1_UE;           // Enable USART
}

// Send a single character via USART2
void USART2_SendChar(char ch) {
    while (!(USART2->SR & USART_SR_TXE));  // Wait until TX buffer is empty
    USART2->DR = (ch & 0xFF);              // Send the character
}

// Simple delay function (approximate for 8MHz SYSCLK)
void DelayMs(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) {
        __NOP();  // Adjust loop iterations based on your clock speed
    }
}