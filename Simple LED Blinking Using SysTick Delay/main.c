/*#include "stm32f10x.h" // CMSIS header

void SysTick_Init(void) {
    // SystemCoreClock = 72MHz ??? ?????
    SysTick->LOAD = 72000 - 1; // 1ms delay
    SysTick->VAL = 0;          // Current value reset
    SysTick->CTRL = 5;         // Enable SysTick, use processor clock, no interrupt
}

void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        // Wait until COUNTFLAG is set
        while((SysTick->CTRL & (1 << 16)) == 0);
    }
}

int main(void) {
    // Enable GPIOC clock
    RCC->APB2ENR |= (1 << 4);
    // Set PC13 as output
    GPIOC->CRH &= ~(0xF << 20); // Clear CNF and MODE bits
    GPIOC->CRH |= (0x2 << 20);  // MODE = 10 (Output 2 MHz), CNF = 00 (General Push-Pull)

    SysTick_Init(); // Initialize SysTick

    while(1) {
        GPIOC->ODR ^= (1 << 13); // Toggle PC13
        delay_ms(500);           // 500ms delay
    }
}
*/
// Button Debouncing Using SysTick
 #include "stm32f10x.h"

// Button GPIO Pin (Assume it's connected to GPIOA pin 0)
#define BUTTON_PIN 0

// Function to initialize SysTick
void SysTick_Init(void) {
    // Assuming 72 MHz system clock
    SysTick->LOAD = 72000 - 1;    // Set the reload register (1ms delay)
    SysTick->VAL = 0;              // Reset the counter
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;  // Enable SysTick timer and interrupt
}

// Delay function using SysTick
void SysTick_Delay(uint32_t delay_ms) {
    SysTick->LOAD = (delay_ms * 72000) - 1;  // Load value for ms delay
    SysTick->VAL = 0;                        // Clear current value
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);  // Wait until the delay is over
}

// Function to initialize GPIO for the button
void GPIO_Init(void) {
    // Enable GPIOA clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Configure PA0 as input with pull-up
    GPIOA->CRL &= ~GPIO_CRL_MODE0;          // Clear mode bits for PA0
    GPIOA->CRL |= GPIO_CRL_CNF0_1;          // Configure PA0 as input with pull-up
    GPIOA->ODR |= GPIO_ODR_ODR0;            // Set PA0 to high (enable pull-up resistor)
}

// Function to read the button state
int Read_Button(void) {
    return (GPIOA->IDR & (1 << BUTTON_PIN)) == 0;  // Button pressed if PA0 is low
}

int main(void) {
    GPIO_Init();       // Initialize GPIO for button
    SysTick_Init();    // Initialize SysTick

    while (1) {
        if (Read_Button()) {
            SysTick_Delay(20);  // Wait for debounce delay

            // Check again after the delay
            if (Read_Button()) {
                // Button is pressed and stable
                // Perform desired action (e.g., send data over USART)
            }
        }
    }
}
