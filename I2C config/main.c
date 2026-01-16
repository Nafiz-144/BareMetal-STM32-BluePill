#include "stm32f10x.h"

void I2C1_GPIO_Init(void) {
    // Enable GPIOB clock (bit 3)
    RCC->APB2ENR |= (1 << 3);

    // Configure PB6 and PB7 as Alternate function open-drain, max speed 50 MHz
    // PB6 = CRL bits 24-27, PB7 = CRL bits 28-31
    GPIOB->CRL &= ~((0xF << 24) | (0xF << 28));      // Clear bits
    GPIOB->CRL |=  ((0xB << 24) | (0xB << 28));      // 0xB = 1011b: AF Open-drain, 50 MHz
}

void I2C1_Init(void) {
    // Enable I2C1 clock (bit 21 on APB1)
    RCC->APB1ENR |= (1 << 21);

    // Reset I2C1 peripheral (optional)
    I2C1->CR1 |= (1 << 15);  // SWRST
    I2C1->CR1 &= ~(1 << 15);

    // Configure peripheral clock frequency (PCLK1) in MHz, here 36 MHz
    I2C1->CR2 = 36;

    // Configure clock control register for 100kHz:
    // CCR = PCLK1 / (2 * I2C_frequency) = 36MHz / (2 * 100kHz) = 180
    I2C1->CCR = 180;

    // Configure TRISE = max rise time / (1/PCLK1) + 1
    // max rise time = 1000 ns, 1/36MHz = 27.7 ns
    // TRISE = 1000 ns / 27.7 ns + 1 ˜ 37
    I2C1->TRISE = 37;

    // Enable I2C1 peripheral
    I2C1->CR1 |= (1 << 0);
}
int main(void) {
    I2C1_GPIO_Init();
    I2C1_Init();

    // Now you can call your SH1106 or SSD1306 functions to communicate

    while (1) {
    }
}
