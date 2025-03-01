#include "stm32f10x.h"

void USART1_Init(void);
void USART1_SendChar(char c);

int main(void) {
    USART1_Init();

    while (1) {
        USART1_SendChar('H');
        USART1_SendChar('e');
        USART1_SendChar('l');
        USART1_SendChar('l');
        USART1_SendChar('o');
        USART1_SendChar('\n');
        for (int i = 0; i < 1000000; i++); // Delay
    }
}

void USART1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // Enable USART1 clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // Enable GPIOA clock

    GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9);
    GPIOA->CRH |= (GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9_0); // PA9 as TX

    USART1->BRR = 0x1D4C; // Baud rate 9600 (assuming 72MHz clock)
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE; // Enable USART1 and TX
}

void USART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE)); // Wait for TX buffer empty
    USART1->DR = c;
}
