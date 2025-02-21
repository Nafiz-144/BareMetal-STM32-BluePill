#include "stm32f10x.h"

void USART1_Init(void);
void USART1_Write(char *data);

int main(void) {
    USART1_Init();
    while (1) {
        USART1_Write("Hello STM32!\r\n");
        for (volatile int i = 0; i < 1000000; i++); // Simple delay
    }
}

void USART1_Init(void) {
    RCC->APB2ENR |= (1 << 2) | (1 << 14); // Enable GPIOA and USART1 clock
    GPIOA->CRH &= ~((0xF << 4) | (0xF << 8));
    GPIOA->CRH |= (0xB << 4) | (0x4 << 8);
    USART1->BRR = 0x1D6; // Baud Rate 9600
    USART1->CR1 |= (1 << 13) | (1 << 3) | (1 << 2);
}

void USART1_Write(char *data) {
    while (*data) {
        while (!(USART1->SR & (1 << 7)));
        USART1->DR = (*data++ & 0xFF);
    }
}
