#include "stm32f10x.h"     // Device header
char chat='A';

int main(void){
	RCC->APB2ENR |=((1<<0)|(1<<2)); //ENABLE APB2 AND GPIOA
	
	RCC->APB1ENR |=(1<<17); // ENABLE UASART2
	GPIOA->CRL &= ~((1<<15)|(1<<14)|(1<<13)|(1<<12)); //RESET PORT A PIN 2
	GPIOA->CRL |=((1<<15)|(1<<13)|(1<<12)); //SET PORT A PIN 2 AS AF PUSH -PULL
	
	
	GPIOA->CRL &= ~((1<<19)|(1<<18)|(1<<17)|(1<<16));
	GPIOA->CRL |=(1<<18);
	
	USART2->BRR |=0XEA6;
	
	USART2->CR1 |=(1<<13);
	USART2 ->CR1 |=(1<<3);
	USART2->CR1 |=(1<<2);
	
while (1) {
    while (!(USART2->SR & (1 << 7))); // Wait until TXE is set
    USART2->DR = chat; // Send character
}
}
	
	
	
	
	
	
