#include "stm32f10x.h"                  // Device header
void DELAY(volatile uint32_t count){
	while(count--){}}


int main(){

RCC->APB2ENR |=RCC_APB2ENR_IOPCEN;
GPIOC->CRH &=~(GPIO_CRH_MODE13|GPIO_CRH_CNF13);
GPIOC->CRH |=(GPIO_CRH_MODE13_0);
GPIOC->CRH &=~(GPIO_CRH_CNF13);
while(1){
GPIOC->ODR ^=(GPIO_ODR_ODR13);
	DELAY(1000);
	

}
	
return 0;
}