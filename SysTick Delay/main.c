#include "stm32f10x.h"    // Device header
void systickdelay(int n);

int main(void){

RCC->APB2ENR |=(1<<4);
	GPIOC->CRH &=0xff0fffff;
	GPIOC->CRH |=(1<<20);
	while(1){
		systickdelay(1000);
		GPIOC->ODR ^= (1<<13);}
	

	
}
void systickdelay(int n){
  SysTick->LOAD =8000;
	SysTick->VAL=0;
	SysTick->CTRL=0x5;
  for(int i=0;i<n;i++){
   while((SysTick->CTRL & 0x10000)==0){}}
	SysTick->CTRL=0;
	

}/*
#include "stm32f10x.h"    // Device header

void systickdelay(int n);

int main(void)
{
    RCC->APB2ENR |= (1 << 4); // Enable GPIOC clock

    GPIOC->CRH &= ~(0xF << 20);  // Clear CNF13[1:0] and MODE13[1:0]
    GPIOC->CRH |= (1 << 20);     // Set MODE13 = 01 (Output mode, max speed 10 MHz)

    while(1)
    {
        systickdelay(1000); // 300ms delay
        GPIOC->ODR ^= (1 << 13); // Toggle PC13
    }
}

void systickdelay(int n)
{
    SysTick->LOAD = 8000; // Reload with number of clocks per millisecond (assuming 8MHz SysTick clock)
    SysTick->VAL = 0; // Clear current value register
    SysTick->CTRL = 0x5; // Enable SysTick (Processor Clock, No Interrupt)

    for (int i = 0; i < n; i++)
    {
        while ((SysTick->CTRL & 0x10000) == 0); // Wait until COUNTFLAG is set
    }
    SysTick->CTRL = 0; // Disable SysTick after delay
}
*/