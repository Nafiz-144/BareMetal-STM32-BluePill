#include "stm32f10x.h"

void SPI1_Init(void);
void USART1_Init(void);
void USART1_SendString(char *str);
void LED_Init(void);
void LED_Toggle(void);
void delayMs(int delay);

int main(void) {
    uint8_t txData = 0x55;
    uint8_t rxData;

    LED_Init();
    SPI1_Init();
    USART1_Init();

    while (1) {
        LED_Toggle();

        SPI1->DR = txData;

        while (!(SPI1->SR & (1 << 0))); // Wait RXNE

        rxData = SPI1->DR;

        if (rxData == txData) {
            USART1_SendString("Success: Data matched\r\n");
        } else {
            USART1_SendString("Fail: Data mismatch\r\n");
        }

        delayMs(1000);
    }
}

void SPI1_Init(void) {
    RCC->APB2ENR |= (1 << 2);   // GPIOA clock
    RCC->APB2ENR |= (1 << 12);  // SPI1 clock

    GPIOA->CRL &= ~((0xF << 20) | (0xF << 24) | (0xF << 28));
    GPIOA->CRL |= (0xB << 20);  // PA5 SCK AF Push-Pull
    GPIOA->CRL |= (0x4 << 24);  // PA6 MISO Input Floating
    GPIOA->CRL |= (0xB << 28);  // PA7 MOSI AF Push-Pull

    SPI1->CR1 = 0;
    SPI1->CR1 |= (1 << 2);  // Master mode
    SPI1->CR1 |= (1 << 1);  // Baud rate fPCLK/4
    SPI1->CR1 |= (1 << 6);  // SPI enable
}

void USART1_Init(void) {
    RCC->APB2ENR |= (1 << 2);    // GPIOA clock
    RCC->APB2ENR |= (1 << 14);   // USART1 clock

    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |= (0xB << 4);    // PA9 AF Push-Pull

    USART1->BRR = 0x1D4C;        // 9600 baud @ 72MHz
    USART1->CR1 |= (1 << 13);    // USART enable
    USART1->CR1 |= (1 << 3);     // TE enable
}

void USART1_SendString(char *str) {
    while (*str) {
        USART1->DR = *str++;
        while (!(USART1->SR & (1 << 6))); // Wait TC
    }
}

void LED_Init(void) {
    RCC->APB2ENR |= (1 << 4);  // GPIOC clock
    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |= (0x1 << 20); // PC13 output push-pull 10MHz
    GPIOC->ODR |= (1 << 13);   // LED off (active low)
}

void LED_Toggle(void) {
    GPIOC->ODR ^= (1 << 13);
}

void delayMs(int delay) {
    for (int i = 0; i < delay * 7200; i++) {
        __NOP();
    }
}
