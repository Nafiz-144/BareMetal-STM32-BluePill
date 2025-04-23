#include <stdint.h>

// Memory map definitions
#define PERIPH_BASE       0x40000000UL
#define APB1PERIPH_BASE   PERIPH_BASE
#define APB2PERIPH_BASE   (PERIPH_BASE + 0x10000UL)
#define AHBPERIPH_BASE    (PERIPH_BASE + 0x20000UL)

// Register definitions
#define RCC_BASE          (AHBPERIPH_BASE + 0x1000UL)
#define GPIOA_BASE        (APB2PERIPH_BASE + 0x0800UL)
#define GPIOB_BASE        (APB2PERIPH_BASE + 0x0C00UL)
#define AFIO_BASE         (APB2PERIPH_BASE + 0x0000UL)
#define EXTI_BASE         (APB2PERIPH_BASE + 0x0400UL)
#define TIM2_BASE         (APB1PERIPH_BASE + 0x0000UL)
#define NVIC_BASE         (0xE000E100UL)
#define SCB_BASE          (0xE000ED00UL)

// Register pointers
#define RCC_APB2ENR       (*(volatile uint32_t*)(RCC_BASE + 0x18))
#define RCC_APB1ENR       (*(volatile uint32_t*)(RCC_BASE + 0x1C))
#define GPIOA_CRL         (*(volatile uint32_t*)(GPIOA_BASE + 0x00))
#define GPIOB_CRH         (*(volatile uint32_t*)(GPIOB_BASE + 0x04))
#define GPIOA_ODR         (*(volatile uint32_t*)(GPIOA_BASE + 0x0C))
#define GPIOB_ODR         (*(volatile uint32_t*)(GPIOB_BASE + 0x0C))
#define AFIO_EXTICR1      (*(volatile uint32_t*)(AFIO_BASE + 0x08))
#define EXTI_IMR          (*(volatile uint32_t*)(EXTI_BASE + 0x00))
#define EXTI_FTSR         (*(volatile uint32_t*)(EXTI_BASE + 0x0C))
#define EXTI_PR           (*(volatile uint32_t*)(EXTI_BASE + 0x14))
#define NVIC_ISER0        (*(volatile uint32_t*)(NVIC_BASE + 0x00))
#define TIM2_CR1          (*(volatile uint32_t*)(TIM2_BASE + 0x00))
#define TIM2_PSC          (*(volatile uint32_t*)(TIM2_BASE + 0x28))
#define TIM2_ARR          (*(volatile uint32_t*)(TIM2_BASE + 0x2C))
#define TIM2_DIER         (*(volatile uint32_t*)(TIM2_BASE + 0x0C))
#define TIM2_SR           (*(volatile uint32_t*)(TIM2_BASE + 0x10))
#define SCB_CPACR         (*(volatile uint32_t*)(SCB_BASE + 0x88))

volatile uint8_t click_count = 0;

int main(void) {
    // 1. System Initialization
    SCB_CPACR |= (0xF << 20);  // Enable FPU if needed
    
    // 2. GPIO Configuration
    // Enable GPIO clocks
    RCC_APB2ENR |= (1 << 2) | (1 << 3);  // IOPAEN + IOPBEN
    
    // Configure PA0 as input with pull-up
    GPIOA_CRL &= ~(0xF << 0);  // Clear CNF0/MODE0
    GPIOA_CRL |= (0x8 << 0);   // Input with pull-up/pull-down
    GPIOA_ODR |= (1 << 0);     // Enable pull-up
    
    // Configure PB8/PB9 as output (10MHz, push-pull)
    GPIOB_CRH &= ~(0xFF << 0); // Clear CNF8/MODE8 + CNF9/MODE9
    GPIOB_CRH |= (0x11 << 0);  // Output, 10MHz
    
    // 3. EXTI Configuration
    // Enable AFIO clock
    RCC_APB2ENR |= (1 << 0);
    
    // Connect PA0 to EXTI0
    AFIO_EXTICR1 &= ~(0xF << 0);
    
    // Configure EXTI0
    EXTI_IMR |= (1 << 0);   // Enable EXTI0
    EXTI_FTSR |= (1 << 0);  // Falling edge trigger
    
    // 4. Timer Configuration (TIM2 for debouncing)
    // Enable TIM2 clock
    RCC_APB1ENR |= (1 << 0);
    
    // Configure TIM2 (10ms interval)
    TIM2_PSC = 7200 - 1;   // 72MHz ? 10kHz
    TIM2_ARR = 100 - 1;    // 10ms timeout
    TIM2_DIER |= (1 << 0); // Update interrupt enable
    
    // 5. NVIC Configuration
    NVIC_ISER0 |= (1 << 6);   // Enable EXTI0_IRQn (position 6)
    NVIC_ISER0 |= (1 << 28);  // Enable TIM2_IRQn (position 28)
    
    while (1) {
        __asm__ volatile("wfi"); // Wait for interrupt
        
        // Main loop does nothing - all work happens in interrupts
    }
}

// Interrupt Service Routines
void EXTI0_IRQHandler(void) __attribute__((interrupt));
void EXTI0_IRQHandler(void) {
    if (EXTI_PR & (1 << 0)) {
        if (click_count == 0) TIM2_CR1 |= (1 << 0); // Start TIM2
        click_count++;
        EXTI_PR = (1 << 0); // Clear pending
    }
}

void TIM2_IRQHandler(void) __attribute__((interrupt));
void TIM2_IRQHandler(void) {
    if (TIM2_SR & (1 << 0)) {
        TIM2_CR1 &= ~(1 << 0); // Stop TIM2
        TIM2_SR &= ~(1 << 0);  // Clear UIF
        
        if (click_count == 1) {
            GPIOB_ODR ^= (1 << 8); // Toggle LED1 (PB8)
        }
        else if (click_count == 2) {
            GPIOB_ODR ^= (1 << 9); // Toggle LED2 (PB9)
        }
        
        click_count = 0;
    }
}

// Default interrupt handlers
void Default_Handler(void) { while(1); }
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));