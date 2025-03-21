#include "stm32f10x.h"                  // Device header
// Define register addresses
#define RCC_APB2ENR   (*(volatile uint32_t*)0x40021018)
#define GPIOA_CRL     (*(volatile uint32_t*)0x40010800)
#define GPIOA_ODR     (*(volatile uint32_t*)0x4001080C)
#define GPIOC_CRH     (*(volatile uint32_t*)0x40011004)
#define GPIOC_IDR     (*(volatile uint32_t*)0x40011008)

#define AFIO_EXTICR4  (*(volatile uint32_t*)0x40010014)
#define EXTI_IMR      (*(volatile uint32_t*)0x40010400)
#define EXTI_FTSR     (*(volatile uint32_t*)0x4001040C)
#define EXTI_PR       (*(volatile uint32_t*)0x40010414)

#define NVIC_ISER1    (*(volatile uint32_t*)0xE000E104)
	
int main(void){
	__disable_irq(); //disable interrupt globally 
	 RCC_APB2ENR |= (1 << 2) | (1 << 4) | (1 << 0);
	 
	GPIOA->CRL &= 0XFF0FFFFF;
	GPIOA->CRL |= (1<<20);
	  // Select Port C for EXTI13
	GPIOC->CRH &=0XFF0FFFFF;
	GPIOC->CRH |=(1<<21);
	 // Unmask EXTI13 (Enable Interrupt Mask)
	  EXTI_IMR |= (1 << 13);
	
 // Select falling edge trigger for EXTI13
    EXTI_FTSR |= (1 << 13);

    // Enable EXTI15_10 interrupt in NVIC (IRQ40 in ISER1)
    NVIC_ISER1 |= (1 << 8);  // (40 - 32) = 8th bit in ISER1

    // Enable interrupts globally
    __enable_irq();

    while(1) {}
}

// Interrupt handler for EXTI15_10 (Handles PC13 Button Press)
void EXTI15_10_IRQHandler(void) {
    if (EXTI_PR & (1 << 13)) {  // Check if EXTI13 triggered
        GPIOA_ODR ^= (1 << 5);   // Toggle PA5 (LED)
        EXTI_PR |= (1 << 13);    // Clear pending bit
    }
}

// SysTick delay function (1ms delay)
void systickDelayMs(int n) {
    SysTick->LOAD = 72000;  // Load value for 1ms (72MHz system clock)
    SysTick->VAL = 0;       // Reset counter
    SysTick->CTRL = 5;      // Enable SysTick, no interrupt

    for (int i = 0; i < n; i++) {
        while ((SysTick->CTRL & 0x10000) == 0);  // Wait for COUNTFLAG
    }

    SysTick->CTRL = 0;  // Disable SysTick
} 
	