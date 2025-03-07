#include "stm32f10x.h"  // Device header for STM32F103C8

// Led : PA5
// Button: PC13

void systickDelayMs(int n);

int counter = 0;

int main(void)
{
    __disable_irq();

    // Enable clock access to GPIOA and GPIOC
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;

    // Configure PA5 as output
    GPIOA->CRL &= ~(0xF << 20); // Clear mode and configuration bits for PA5
    GPIOA->CRL |= (0x1 << 20);  // Set PA5 as output, max speed 10 MHz
    GPIOA->CRL &= ~(0x3 << 22); // Set PA5 as general purpose output push-pull

    // Configure PC13 as input
    GPIOC->CRH &= ~(0xF << 20); // Clear mode and configuration bits for PC13
    GPIOC->CRH |= (0x4 << 20);  // Set PC13 as input with pull-up/pull-down

    // Select Port C for EXTI13
    AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PC;

    // Unmask EXTI13
    EXTI->IMR |= EXTI_IMR_MR13;

    // Select falling edge trigger for EXTI13
    EXTI->FTSR |= EXTI_FTSR_TR13;

    // Enable EXTI15_10 interrupt in NVIC
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    __enable_irq();

    while(1){}
}

void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & EXTI_PR_PR13) {
        // Toggle PA5
        GPIOA->ODR ^= GPIO_ODR_ODR5;
        // Clear pending bit
        EXTI->PR = EXTI_PR_PR13;
    }
}

void systickDelayMs(int n) {
    SysTick->LOAD = 72000; // Reload with number of clocks per millisecond (72 MHz clock)
    SysTick->VAL = 0;      // Clear current value register
    SysTick->CTRL = 0x5;   // Enable SysTick

    for (int i = 0; i < n; i++) {
        // Wait until the COUNT flag is set
        while ((SysTick->CTRL & 0x10000) == 0);
    }

    SysTick->CTRL = 0;
}