#include "stm32f10x.h"

// === USART1 Initialization ===
void USART1_Init(void) {
    // Enable clocks for GPIOA, USART1, AFIO
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // PA9 (TX) = Alternate Function Push-Pull
    GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
    GPIOA->CRH |=  (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_MODE9_1); // CNF=10, MODE=11 (50 MHz)

    // PA10 (RX) = Input floating
    GPIOA->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
    GPIOA->CRH |=  GPIO_CRH_CNF10_0; // CNF=01, MODE=00

    // Baud rate: 72MHz / 9600 = 7500 = 0x1D4C
    USART1->BRR = 0x1D4C;  // Baud = 9600

    // Enable USART TX, RX and USART itself
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
    USART1->CR1 |= USART_CR1_UE;
}

// === Send a single character ===
void USART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE)); // Wait until TX is empty
    USART1->DR = c;
}

// === Receive a single character ===
char USART1_ReceiveChar(void) {
    while (!(USART1->SR & USART_SR_RXNE)); // Wait until RX is full
    return USART1->DR;
}

// === Send a null-terminated string ===
void USART1_SendString(const char* str) {
    while (*str) {
        USART1_SendChar(*str++);
    }
}

// === Main Program ===
int main(void) {
    USART1_Init();
    USART1_SendString("New Project With USART.\r\n");

    while (1) {
        char c = USART1_ReceiveChar(); // Wait for data from PC
        USART1_SendChar(c);            // Echo it back
    }
}

