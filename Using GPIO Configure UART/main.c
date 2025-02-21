#include "stm32f10x.h" // Device header

void USART2_init(void);
void USART2_write(char ch);
void USART2_writeString(char *str);
void Delay(volatile uint32_t count);

int main(void) {
    USART2_init(); // ? Initialize USART2 correctly

    while (1) {
        USART2_writeString(" STM32!\r\n"); // ? Send message
        Delay(1000000); // ? Delay for readability
    }
}

// ? USART2 Initialization Function
void USART2_init(void) {
    // 1. ? Enable USART2 Clock (APB1)
    RCC->APB1ENR |= (1 << 17);

    // 2. ? Enable GPIOA Clock (USART2 TX is PA2, not PB3)
    RCC->APB2ENR |= (1 << 2);

    // 3. ? Configure PA2 (TX) as Alternate Function Push-Pull (50MHz)
    GPIOA->CRL &= ~((1<<11)|(1<<10)|(1<<9)|(1<<8)); // Reset PA2 (Bits 8-11)
    GPIOA->CRL |= ((1<<11)|(1<<9)|(1<<8));  // Set PA2 as AF Push-Pull

    // 4. ? Configure Baud Rate (9600 @ 8MHz Clock)
    USART2->BRR = 0x341; // 9600 baud for 8MHz system clock

    // 5. ? Enable Transmitter
    USART2->CR1 |= (1 << 3); 

    // 6. ? Enable USART2
    USART2->CR1 |= (1 << 13); 
}

// ? Send a Single Character via USART2
void USART2_write(char ch) {
    while (!(USART2->SR & (1 << 7))); // Wait until TX buffer is empty
    USART2->DR = (ch & 0xFF); // Send Data
}

// ? Send a String via USART2
void USART2_writeString(char *str) {
    while (*str) {
        USART2_write(*str++);
    }
}

// ? Simple Delay Function
void Delay(volatile uint32_t count) {
    while(count--);
}
