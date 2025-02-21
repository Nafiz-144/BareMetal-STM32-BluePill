#include "stm32f10x.h" // Device header

void USART2_Init(void);
void USART_write(char ch);
void USART_writeString(char *str);
void Delay(volatile uint32_t count);

int main(void) {
    USART2_Init(); // Initialize USART2

    while(1) {
        USART_writeString("Hello, STM32!\r\n"); // Send "Hello, STM32!"
        Delay(1000000); // Delay to slow down output
    }
}

// USART2 Initialization Function
void USART2_Init(void) {
    // 1. Enable USART2 Clock (APB1)
    RCC->APB1ENR |= (1 << 17); 

    // 2. Enable GPIOA Clock (APB2)
    RCC->APB2ENR |= (1 << 2);  

    // 3. Configure PA2 (TX) as Alternate Function Push-Pull (50 MHz)
    GPIOA->CRL &= ~((1<<11)|(1<<10)|(1<<9)|(1<<8)); // Reset PA2 (Bits 8-11)
    GPIOA->CRL |= ((1<<11)|(1<<9)|(1<<8));  // Set PA2 as AF Push-Pull

    // 4. Configure Baud Rate (9600 @ 8MHz Clock)
    USART2->BRR = 0x341; // 9600 baud for 8MHz system clock

    // 5. Enable Transmitter
    USART2->CR1 |= (1 << 3); 

    // 6. Enable USART2
    USART2->CR1 |= (1 << 13); 
}

// Send a Single Character via USART2
void USART_write(char ch) {
    while (!(USART2->SR & (1 << 7))); // Wait until TX buffer is empty
    USART2->DR = (ch & 0xFF); // Send Data
}

// Send a String via USART2
void USART_writeString(char *str) {
    while (*str) {
        USART_write(*str++);
    }
}

// Delay Function
void Delay(volatile uint32_t count) {
    while(count--);
}
