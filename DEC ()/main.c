#include "stm32f10x.h"                  // Device header
int main (void){
	
	RCC->APB2ENR |=4;
	RCC->APB2ENR |=0x10;
	GPIOA->CRL &=0xFFFFFFF0;
	GPIOA->CRL |=0x8;
	
	
	}