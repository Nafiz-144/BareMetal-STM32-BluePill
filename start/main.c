#include "stm32f10x.h"                  // Device header

void delay(void);
int main(void){
	RCC->APB2ENR |= 0x10;
	GPIOC->CRH &= 0xff0fffff;
	GPIOC->CRH |= 0x00300000;
	GPIOC->ODR |= 0x2000;
	while(1){
		
	GPIOC->ODR |= 0x2000;
		delay();
		GPIOC->ODR &=~ 0x2000;
		delay();}
	
	//return 0;
}
void delay(void){
	for(int i=0;i<100;i++){}
	}

	