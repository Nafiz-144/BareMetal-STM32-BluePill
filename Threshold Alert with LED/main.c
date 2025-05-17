/*#include "stm32f10x.h"
#include <stdio.h>

void USART1_Init(void);
void USART1_SendChar(char c);
void USART1_SendString(char *str);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
uint32_t read_distance_cm(void);

void GPIO_Init(void) {
    // Enable GPIOA (bit 2) and USART1 (bit 14) clocks
    RCC->APB2ENR |= (1 << 2) | (1 << 14);

    // PA0 = TRIG (output), PA1 = ECHO (input)
    GPIOA->CRL &= ~((0xF << 0) | (0xF << 4)); // Clear MODE/CNF for PA0, PA1
    GPIOA->CRL |= ( (1 << 1) << 0 );          // PA0: MODE = 10 (2MHz), CNF = 00 (Output push-pull)
    GPIOA->CRL |= ( (1 << 2) << 4 );          // PA1: MODE = 00, CNF = 01 (Input floating)

    // PA9 = USART1_TX
    GPIOA->CRH &= ~(0xF << 4);                // Clear PA9 config
    GPIOA->CRH |= ((1 << 3) | (1 << 1) | (1 << 0)) << 4; // MODE = 11 (50MHz), CNF = 10 (Alt push-pull)
}

int main(void) {
    SystemInit();
    GPIO_Init();
    USART1_Init();

    USART1_SendString("HC-SR04 Initialized...\r\n");

    while (1) {
        unsigned long distance = read_distance_cm();
        char buffer[50];
        sprintf(buffer, "Distance: %lu cm\r\n", distance);
        USART1_SendString(buffer);
        delay_ms(500);
    }
}

uint32_t read_distance_cm(void) {
    // Send 10us pulse on TRIG
    GPIOA->BSRR = (1 << 0);   // Set PA0 HIGH
    delay_us(10);
    GPIOA->BRR = (1 << 0);    // Set PA0 LOW

    // Wait for ECHO to go HIGH
    while (!(GPIOA->IDR & (1 << 1)));

    // Start timing
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL = 0;
    SysTick->CTRL = (1 << 0) | (1 << 2); // Enable, use processor clock, no interrupt

    uint32_t start = SysTick->VAL;
    while (GPIOA->IDR & (1 << 1));       // Wait for ECHO to go LOW
    uint32_t end = SysTick->VAL;

    SysTick->CTRL = 0; // Stop SysTick

    uint32_t time_us = (start > end) ? (start - end) : (0xFFFFFF - end + start);
    time_us = time_us / 8; // Adjust for 72MHz clock

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
    USART1->BRR = (72 * 1000000) / 9600; // Dynamically calculate BRR for 72MHz, 9600 baud
    USART1->CR1 |= (1 << 3) | (1 << 13); // TE (bit 3), UE (bit 13)
}

void USART1_SendChar(char c) {
    while (!(USART1->SR & (1 << 7))); // Wait until TXE (bit 7) is set
    USART1->DR = c;
}

void USART1_SendString(char *str) {
    while (*str) {
        USART1_SendChar(*str++);
    }
}
*/
#include "stm32f10x.h"
#include <stdio.h>

// Function Prototypes
void GPIO_Init(void);
void USART1_Init(void);
void USART1_SendChar(char c);
void USART1_SendString(char *str);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);
void TIM2_Init(void);
uint32_t read_distance_cm(void);

// GPIO Configuration
void GPIO_Init(void) {
    RCC->APB2ENR |= (1 << 2) | (1 << 14); // Enable GPIOA and USART1 clocks

    // PA0 = TRIG (output), PA1 = ECHO (input)
    GPIOA->CRL &= ~((0xF << 0) | (0xF << 4)); // Clear settings
    GPIOA->CRL |= (0x2 << 0);                // PA0 = Output push-pull
    GPIOA->CRL |= (0x4 << 4);                // PA1 = Input floating

    // PA9 = USART1_TX
    GPIOA->CRH &= ~(0xF << 4);               // Clear PA9
    GPIOA->CRH |= (0xB << 4);                // PA9 = Alt func push-pull, 50MHz
}

// TIM2 Initialization (1 �s tick)
void TIM2_Init(void) {
    RCC->APB1ENR |= (1 << 0);   // Enable TIM2 clock

    TIM2->PSC = 71;             // 72MHz / (71+1) = 1 MHz ? 1 �s per tick
    TIM2->ARR = 0xFFFF;         // Max count
    TIM2->CR1 = 1;              // Enable counter
}

// Measure Distance using HC-SR04
uint32_t read_distance_cm(void) {
    GPIOA->BSRR = (1 << 0);     // TRIG = HIGH
    delay_us(10);
    GPIOA->BRR = (1 << 0);      // TRIG = LOW

    while (!(GPIOA->IDR & (1 << 1))); // Wait for ECHO HIGH

    TIM2->CNT = 0;              // Reset timer

    while (GPIOA->IDR & (1 << 1)); // Wait for ECHO LOW

    uint32_t time_us = TIM2->CNT;

    return time_us / 58;        // Convert to cm
}

// Microsecond delay using NOP loop
void delay_us(uint32_t us) {
    for (uint32_t i = 0; i < us * 8; i++) {
        __NOP();
    }
}

// Millisecond delay using delay_us
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        delay_us(1000);
    }
}

// USART1 Initialization
void USART1_Init(void) {
    USART1->BRR = 0x1D4C; // 72MHz / 9600 baud
    USART1->CR1 |= USART_CR1_TE | USART_CR1_UE;
}

// Send one character
void USART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

// Send string
void USART1_SendString(char *str) {
    while (*str) {
        USART1_SendChar(*str++);
    }
}

// Main Program
int main(void) {
    SystemInit();
    GPIO_Init();
    USART1_Init();
    TIM2_Init();

    USART1_SendString("HC-SR04 Initialized with TIM2...\r\n");

    while (1) {
        uint32_t distance = read_distance_cm();
        char buffer[50];
        sprintf(buffer, "Distance: %lu cm\r\n", distance);
        USART1_SendString(buffer);
        delay_ms(500);
    }
}
