// STM32F103C8 - Slave
#include "stm32f10x.h"

void SPI1_Slave_Init(void);
void LED_Init(void);
void toggle_LED(void);

int main(void) {
    SPI1_Slave_Init();
    LED_Init();

    while (1) {
        if (SPI1->SR & (1<<0)) {  // RXNE
            uint8_t data = SPI1->DR; // Read data
            if (data == 0x55) {
                toggle_LED();        // Toggle LED on match
            }
        }
    }
}

void SPI1_Slave_Init(void) {
    RCC->APB2ENR |= (1<<2) | (1<<12); // Enable GPIOA & SPI1

    // PA5 (SCK), PA7 (MOSI), PA4 (NSS) as input floating
    GPIOA->CRL &= ~(0xFF << 20); // Clear PA5, PA7
    GPIOA->CRL |= (0x4 << 20) | (0x4 << 28); // Input floating
    GPIOA->CRL &= ~(0xF << 16);
    GPIOA->CRL |= (0x4 << 16); // NSS as input floating

    // PA6 (MISO) as AF Push-Pull
    GPIOA->CRL &= ~(0xF << 24);
    GPIOA->CRL |= (0xB << 24);

    SPI1->CR1 = 0;           // Slave mode, default config
    SPI1->CR1 |= (1<<6);     // Enable SPI
}

void LED_Init(void) {
    RCC->APB2ENR |= (1<<4);         // Enable GPIOC
    GPIOC->CRH &= ~(0xF << 20);     // Clear PC13
    GPIOC->CRH |= (0x2 << 20);      // PC13 as push-pull output
    GPIOC->BSRR = (1<<13);          // Turn off LED initially
}

void toggle_LED(void) {
    GPIOC->ODR ^= (1<<13);          // Toggle PC13
}
