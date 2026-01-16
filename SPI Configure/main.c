#include "stm32f10x.h"

void SPI1_Master_Init(void);
uint8_t SPI1_TransmitReceive(uint8_t data);
void delayMs(int ms);
void LED_Init(void);
void toggle_LED(void);

int main(void) {
    SPI1_Master_Init();
    LED_Init();

    while (1) {
        uint8_t received = SPI1_TransmitReceive(0x55);  // Send 0x55, receive byte
        if (received == 0xAA) {
            toggle_LED();
        }
        delayMs(1000);
    }
}

void SPI1_Master_Init(void) {
    RCC->APB2ENR |= (1<<2) | (1<<12); // Enable GPIOA & SPI1 clocks

    // PA5 (SCK), PA7 (MOSI), PA4 (NSS) as AF Push-Pull
    GPIOA->CRL &= ~(0xFF << 20); // Clear PA5 and PA7
    GPIOA->CRL |= (0xB << 20) | (0xB << 28); // AF PP, 50MHz
    GPIOA->CRL &= ~(0xF << 16); // Clear PA4
    GPIOA->CRL |= (0x3 << 16);  // PA4 as output push-pull (manual NSS)

    // PA6 (MISO) as input floating
    GPIOA->CRL &= ~(0xF << 24);
    GPIOA->CRL |= (0x4 << 24);

    SPI1->CR1 = (1<<2) | (1<<1) | (1<<0);  // Master, SSM, SSI
    SPI1->CR1 |= (1<<6);                  // Enable SPI
}

uint8_t SPI1_TransmitReceive(uint8_t data) {
    GPIOA->BSRR = (1<<4) << 16;         // NSS LOW
    while (!(SPI1->SR & (1<<1)));       // Wait until TXE
    SPI1->DR = data;
    while (!(SPI1->SR & (1<<0)));       // Wait until RXNE
    uint8_t received = SPI1->DR;        // Read received data
    while (SPI1->SR & (1<<7));          // Wait until not busy
    GPIOA->BSRR = (1<<4);               // NSS HIGH
    return received;
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

void delayMs(int ms) {
    for (int i = 0; i < ms * 8000; i++) __NOP();
}
