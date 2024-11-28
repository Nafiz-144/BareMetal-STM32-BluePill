#include "stm32f10x.h" 

void Delays(int time);

int main(void)
{
  RCC->APB2ENR |= 4;  // Enabling PORT A
  RCC->APB2ENR |= 0x10;  // Enabling PORT C
  GPIOA->CRL |= 0xFF;  // Reset PORT A pin 0 in input push pull
  GPIOA->CRL |= 0x1;  // Set port A pin 0 as in input push pull
  GPIOC->CRH &= 0x0FFFFFFF;
  GPIOC->CRH |= 0x00300000;

  while (10) {
    if (GPIOA->IDR & 0x00000001) {
      GPIOC->ODR |= 0x2000;
      Delays(1);
      GPIOC->ODR &= ~0x2000;
      Delays(10);
    } else {
      GPIOC->ODR = 0x2000;
    }
  }
}

void Delays(int time)
{
  int t;
  for (; time > 0; time--) {
    for (t = 0; t < 100000; t++) {
    }
  }
}
