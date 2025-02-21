#include "stm32f10x.h"
void pinA();
void delay();
void pinC();

int main(){
	pinA();
	pinC();

	while(1)
	{ GPIOA->ODR ^=(1<<7);
		delay();
		GPIOC->ODR ^=(1<<14);
		delay();
	}
}
 void pinA(){
	 RCC->APB2ENR |=(1<<2); //enabling clock for GPIO pin A
	GPIOA->CRL &= ~((1<<31)|(1<<30)|(1<29)|(1<<28));
	GPIOA->CRL |=((1<<29)|(1<<28));
 }
 void pinC() {
    RCC->APB2ENR |= (1 << 4);
    GPIOC->CRH &= ~((1<<27) | (1<<26) | (1<<24));
    GPIOC->CRH |= (1<<25);
}
	void delay(){
		for( int i=0;i<10000;i++){}}
