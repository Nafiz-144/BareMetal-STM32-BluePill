#include "stm32f10x.h"                  // Device header
void usart2_init(void);
void USART_SEND(char ch);
void delay_ms(int delay);
int main(){
usart2_init();
while(1){
USART_SEND('e');
	delay_ms(1000);
USART_SEND('k');
delay_ms(100);
}

}

void usart2_init(){
	
	RCC->APB2ENR |=(1<<2);
	RCC->APB1ENR |=(1<<17);
	GPIOA->CRL &=~(0XF<<8);
	GPIOA->CRL |=(0XB<<8);
	USART2->BRR= 0X1D4C;
	USART2->CR1 |=(1<<3);
	USART2->CR1 |=(1<<13);



}

void USART_SEND(char ch){
while((USART2->SR &(1<<7))==0){}
	USART2->DR=ch;

}
void delay_ms(int delay){

	int i;
	for(;delay>0;delay--){
	for( i=0;i<3126;i++){}
	}
	
}