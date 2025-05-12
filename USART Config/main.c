#include "stm32f10x.h"

void USART2_Init(void);
void USART2_Write(char ch);
char USART2_Read(void);
void USART2_WriteString(const char *str);

int main(void) {
    USART2_Init();
    USART2_WriteString("STM32 Ready!\r\n");

    while (1) {
        char c = USART2_Read();   // Receive from FTDI or ESP32
        USART2_Write(c);          // Echo back
    }
}

void USART2_Init(void) {
    // Enable GPIOA, AFIO, and USART2 clocks
    RCC->APB2ENR |= (1 << 2);  // IOPAEN
    RCC->APB2ENR |= (1 << 0);  // AFIOEN
    RCC->APB1ENR |= (1 << 17); // USART2EN

    // Configure PA2 (TX) as alternate function push-pull, 10 MHz
    // Configure PA3 (RX) as input floating
    GPIOA->CRL &= ~((0xF << 8) | (0xF << 12)); // Clear bits for PA2 and PA3
    GPIOA->CRL |= (0xA << 8);   // PA2: MODE=10 (Output 2MHz), CNF=10 (AF Push-Pull)
    GPIOA->CRL |= (0x4 << 12);  // PA3: MODE=00, CNF=01 (Floating Input)

    // Baud rate = 9600, for 8MHz clock: BRR = 8000000 / (16*9600) ˜ 52.08 ? 0x341
    USART2->BRR = 0x0341;

    // Enable USART2: TE (bit 3), RE (bit 2), UE (bit 13)
    USART2->CR1 = (1 << 3) | (1 << 2) | (1 << 13);
}

void USART2_Write(char ch) {
    while (!(USART2->SR & (1 << 7))); // Wait for TXE (bit 7)
    USART2->DR = ch;
}

char USART2_Read(void) {
    while (!(USART2->SR & (1 << 5))); // Wait for RXNE (bit 5)
    return (char)(USART2->DR & 0xFF);
}

void USART2_WriteString(const char *str) {
    while (*str) {
        USART2_Write(*str++);
    }
}
