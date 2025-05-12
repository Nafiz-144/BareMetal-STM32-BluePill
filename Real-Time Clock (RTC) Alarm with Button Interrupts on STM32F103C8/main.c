#include "stm32f10x.h"

volatile uint8_t alarm_triggered = 0;

// USART1 init
void USART1_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |= (0xB << 4);  // PA9 TX: 50MHz, AF Push-Pull

    USART1->BRR = 0x1D4C; // 72MHz / 9600
    USART1->CR1 = USART_CR1_TE | USART_CR1_UE;
}

void USART1_SendChar(char c) {
    while (!(USART1->SR & USART_SR_TXE));
    USART1->DR = c;
}

void USART1_SendString(char *s) {
    while (*s) USART1_SendChar(*s++);
}

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 8000; i++) __NOP();
}

// LED on PA5
void LED_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~(0xF << 20);
    GPIOA->CRL |= (0x2 << 20);  // Output 2MHz, Push-Pull
}

// Button on PC13
void Button_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;

    GPIOC->CRH &= ~(0xF << 20);
    GPIOC->CRH |= (0x2 << 20);  // Input with pull-up/down
    GPIOC->ODR |= (1 << 13);    // Pull-up

    AFIO->EXTICR[3] |= (0x2 << 4); // EXTI13 = PC13
    EXTI->IMR |= (1 << 13);
    EXTI->FTSR |= (1 << 13);

    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// RTC init with LSI
void RTC_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
    PWR->CR |= PWR_CR_DBP;

    RCC->CSR |= RCC_CSR_LSION;
    while (!(RCC->CSR & RCC_CSR_LSIRDY));

    RCC->BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSI;

    RTC->CRL |= RTC_CRL_CNF;
    RTC->PRLL = 39999;  // 40kHz / (39999+1) = 1Hz
    RTC->CRL &= ~RTC_CRL_CNF;
    while (!(RTC->CRL & RTC_CRL_RTOFF));
}

// RTC alarm setup
void RTC_SetAlarm(uint32_t seconds) {
    uint32_t current = ((RTC->CNTH << 16) | RTC->CNTL);
    uint32_t alarm = current + seconds;

    RTC->CRL |= RTC_CRL_CNF;

    RTC->ALRH = alarm >> 16;
    RTC->ALRL = alarm & 0xFFFF;
    RTC->CRH |= RTC_CRH_ALRIE;

    RTC->CRL &= ~RTC_CRL_CNF;
    while (!(RTC->CRL & RTC_CRL_RTOFF));
}

// EXTI + NVIC setup for RTC
void RTC_NVIC_Init(void) {
    EXTI->IMR |= (1 << 17);
    EXTI->RTSR |= (1 << 17);
    NVIC_EnableIRQ(RTCAlarm_IRQn);
    NVIC_SetPriority(RTCAlarm_IRQn, 0);
}

// RTC Alarm IRQ
void RTC_Alarm_IRQHandler(void) {
    if (RTC->CRL & RTC_CRL_ALRF) {
        RTC->CRL &= ~RTC_CRL_ALRF;
        EXTI->PR |= (1 << 17);

        GPIOA->BSRR = (1 << 5);  // LED ON
        USART1_SendString("RTC Alarm Triggered!\r\n");

        alarm_triggered = 1;
    }
}

// Button IRQ
void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1 << 13)) {
        EXTI->PR |= (1 << 13);
        USART1_SendString("Button Pressed - Reset Alarm\r\n");
        RTC_SetAlarm(5);
    }
}

// Main
int main(void) {
    USART1_Init();
    LED_Init();
    Button_Init();
    RTC_Init();
    RTC_NVIC_Init();

    RTC_SetAlarm(10);
    USART1_SendString("RTC Alarm + LED Ready\r\n");

    while (1) {
        if (alarm_triggered) {
            alarm_triggered = 0;
            delay_ms(10);  // Now visible
            GPIOA->BRR = (1 << 5);  // LED OFF
        }
    }
}
