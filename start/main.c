#include "stm32f10x.h"                  // Device header


int main(void){
	RCC->APB2ENR |=0x10;
	GPIOC->CRH &=0xff0fffff;
	GPIOC->CRH |=0x00300000;
	
	
	
	//return 0;
}