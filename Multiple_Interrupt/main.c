#include "stm32f10x.h"

void SysTick_DelayMs(int n);

volatile int pb10_counter = 0;
volatile int pc13_counter = 0;

int main(void) {
    // Enable clocks
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;  // Enable GPIOB clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;  // Enable GPIOC clock
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;  // Enable AFIO clock

    // Configure PC13 as input with pull-up
    GPIOC->CRH &= ~(0xF << (5 * 4));  // Clear bits
    GPIOC->CRH |= (0x8 << (5 * 4));   // Input with pull-up/pull-down
    GPIOC->ODR |= (1 << 13);          // Enable pull-up

    // Configure PB10 as input with pull-up
    GPIOB->CRH &= ~(0xF << (2 * 4));  // Clear bits
    GPIOB->CRH |= (0x8 << (2 * 4));   // Input with pull-up/pull-down
    GPIOB->ODR |= (1 << 10);          // Enable pull-up

    // Configure EXTI for PC13
    AFIO->EXTICR[3] &= ~(0xF << 4);  // Clear EXTI13 bits
    AFIO->EXTICR[3] |= (0x2 << 4);   // Select PortC for EXTI13
    EXTI->IMR |= (1 << 13);          // Unmask EXTI13
    EXTI->FTSR |= (1 << 13);         // Falling edge trigger

    // Configure EXTI for PB10
    AFIO->EXTICR[2] &= ~(0xF << 8);  // Clear EXTI10 bits
    AFIO->EXTICR[2] |= (0x1 << 8);   // Select PortB for EXTI10
    EXTI->IMR |= (1 << 10);          // Unmask EXTI10
    EXTI->FTSR |= (1 << 10);         // Falling edge trigger

    // Enable EXTI15_10 IRQ
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    __enable_irq();

    while (1) {
        // Main loop
    }
}

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1 << 13)) {  // Check if EXTI13 triggered
        pc13_counter++;
        EXTI->PR = (1 << 13);    // Clear pending bit
    }
    if (EXTI->PR & (1 << 10)) {  // Check if EXTI10 triggered
        pb10_counter++;
        EXTI->PR = (1 << 10);    // Clear pending bit
    }
}

void SysTick_DelayMs(int n) {
    SysTick->LOAD = 16000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = 5;

    for (int i = 0; i < n; i++) {
        while ((SysTick->CTRL & 0x10000) == 0);
    }
    SysTick->CTRL = 0;
}
