#include "stm32f10x.h"

void init_GP(GPIO_TypeDef *PORT, unsigned short PIN, unsigned short DIR, unsigned short OPT) {
    volatile uint32_t *CR;
    unsigned short tPin = PIN;
    
    if (PIN > 7) {
        tPin -= 8;
        CR = &PORT->CRH;  // High register for pins 8-15
    } else {
        CR = &PORT->CRL;  // Low register for pins 0-7
    }

    // Enable Clock for GPIO
    if (PORT == GPIOA) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    } else if (PORT == GPIOB) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    } else if (PORT == GPIOC) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    }

    // Configure Mode and Configuration
    *CR &= ~(0xF << (tPin * 4)); // Clear existing settings
    *CR |= ((DIR << (tPin * 4)) | (OPT << (tPin * 4 + 2)));
}

int R_GP(GPIO_TypeDef *PORT, unsigned short PIN) {
    return (PORT->IDR & (1 << PIN)) ? 1 : 0;
}

void W_GP(GPIO_TypeDef *PORT, unsigned short PIN, unsigned short STATUS) {
    if (STATUS) {
        PORT->ODR |= (1 << PIN);
    } else {
        PORT->ODR &= ~(1 << PIN);
    }
}

void toggle_GP(GPIO_TypeDef *PORT, unsigned short PIN) {
    if (R_GP(PORT, PIN)) {
        W_GP(PORT, PIN, 0);
    } else {
        W_GP(PORT, PIN, 1);
    }
}

void PINc(unsigned short PIN, unsigned short STATUS) {
    if (STATUS) {
        GPIOC->ODR |= (1 << PIN);
    } else {
        GPIOC->ODR &= ~(1 << PIN);
    }
}

void B_init(void) {
    init_GP(GPIOC, 13, 2, 0);  // 2 = Output mode, 50MHz; 0 = Push-Pull
}

void BLED(unsigned short state) {
    W_GP(GPIOC, 13, state);
}
