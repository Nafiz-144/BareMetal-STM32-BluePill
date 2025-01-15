#include "stm32f10x.h"

void delay(int rep);

int main(void)
{
  RCC->APB2ENR |= 0x10;  // 0b10000
  GPIOA->CRH &= 0xFFFF0FFF;
  GPIOC->CRH |= 0x00300000;
  GPIOC->ODR |= 0x2000;

  while (1) {
    GPIOC->ODR |= 0x2000;
    delay(10);
    GPIOC->ODR &= ~0x2000;
    delay(10);
  }
}

void delay(int rep)
{
  for (; rep > 0; rep--) {
    for (int i = 0; i < 100000; i++) {
    }
  }
}