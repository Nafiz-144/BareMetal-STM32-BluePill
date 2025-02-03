#include "stm32f10x.h"

void Delays (int time);

int main(void)
{
    RCC->APB2ENR |= 0x10; // Enabling PORT A
    GPIOA->CRL &= 0xFFFFFFF0; // Reset the Port A PIN 0
    GPIOA->CRL = 0x8; // Set port A pin 0 as in Input push pull

    GPIOC->CRH &= 0xFFFFFFF0; // Reset the pin 13.
    GPIOC->CRH = 0x00300000; // Set PIN 13 as output

    while (1)
    {
        if (GPIOA->IDR & 0x00000001) // Checking status of PIN portA
        {
            GPIOC->ODR ^= 0x2000; // Toggle the PIN state
            Delays (10);
            Delays (10);
        }
        else
        {
            GPIOC->ODR = 0x2000; // Set the PIN 13 port C high
        }
    }
}

void Delays (int time) // Random delay function
{
    int t;

    for(;time>0;time--)
    {
        for(t=0;t<100000;t++);
    }
}