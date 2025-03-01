#include "stm32f10x.h"                  // Device header
int main(){
	
	RCC->APB2ENR |= (1<<4);
	GPIOC->CRH &= ~((1<<31) | (1<<30) | (1<<29) | (1<<28));
	
	GPIOC->CRH |= (1<<28);
	while(1){
	
	GPIOC -> ODR ^=(1<<15);
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	/*RCC->APB2ENR |=(1<<2);
	GPIOA->CRL &= ~((1<<23)|(1<<22)|(1<<21)|(1<<20));
	GPIOA->CRL |=(1<<21);
	while(1){
		GPIOA->ODR ^=(1<<5);}*/
	return 0;
}