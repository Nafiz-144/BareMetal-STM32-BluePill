
/* -------Smart Environmental Alert System------*/
#include "stm32f10x.h"                  


void gpio_init(){

RCC->APB2ENR |=(1<<2); //ENABLE CLOCK FOR GPIOA
	/*// DHT11 Data Line
	//PA1 CONFIG AS OUTPUT OPEN DRAIN
	GPIOA->CRL &=~( (1<<7)|(1<<6)|(1<<5)|(1<<4)); //RESET PIN
	GPIOA->CRL |=((1<<7)|(1<<6)|(1<<5)); //SET PIN AS OP(2mhz)
	
	//PA1 CONFIG AS INPUT FLOATING
	GPIOA->CRL &=~( (1<<7)|(1<<6)|(1<<5)|(1<<4)); //RESET PIN
	GPIOA->CRL |=(1<<6); // SET PIN AS INPUT*/
	
	//HC-SR04 Trigger

 //PA2 CONFIG AS OUTPUT PUSH PULL
	GPIOA->CRL &=~((1<<11)|(1<<10)|(1<<9)|(1<<8)); //RESET PIN
	GPIOA->CRL |= (1<<9); //SET PIN AS OP(2mhz)
	
	// HC-SR04 Echo
	
	// PA3 CONFIG AS INPUT FLOATING
	GPIOA->CRL &=~((1<<15)|(1<<14)|(1<<13)|(1<<12)); //RESET PIN
	GPIOA->CRL |=(1<<14); //SET PIN AS IN
	
	//FOR THE LED
	
	//PA5 CONFIG AS OUTPUT PUSH PULL
	GPIOA->CRL &=~((1<<23)|(1<<22)|(1<<21)|(1<<20)); // RESET PIN
	GPIOA->CRL |=(1<<21); // SET PIN AS OP
	
	}
void tim2_init(void){

	RCC->APB1ENR |=(1<<0); //ENABLE TIM2
	TIM2->PSC =71; //PRESCALER
	TIM2->ARR =0XFFFF;
	TIM2->CNT=0; // RESET COUNTER
	TIM2->CR1 |=(1<<0); //ENABLE TIM2
	
	}

void delays_us_tim2(uint16_t us){
		TIM2->CNT =0;
	while(TIM2->CNT<us); }


void delay_ms(uint16_t ms ){
	while(ms--)
		delays_us_tim2(1000);
}
// ---DHT11 FUNCTION
 

void dht11_start(void){
	
	//PA1 CONFIG AS OUTPUT OPEN DRAIN
	GPIOA->CRL &=~( (1<<7)|(1<<6)|(1<<5)|(1<<4)); //RESET PIN
	GPIOA->CRL |=((1<<7)|(1<<6)|(1<<5)); //SET PIN AS OP(2mhz)
	GPIOA->ODR &=~(1<<1); // PULL LOW
  delay_ms(20); //DELAY FOR 18 MS
	GPIOA->ODR |=(1<<1); // PULL HIGH
	
	//PA1 CONFIG AS INPUT FLOATING
	GPIOA->CRL &=~( (1<<7)|(1<<6)|(1<<5)|(1<<4)); //RESET PIN
	GPIOA->CRL |=(1<<6); // SET PIN AS INPUT

}

//---- DHT11 CHECK RESPONSE 
uint8_t dht11_response(void){
   delays_us_tim2(40);
   if(!(GPIOA->IDR & (1<<1))){
	 delays_us_tim2(80);
	  
   if(GPIOA->IDR &(1<<1)){
	
    delays_us_tim2(80);
		 return 1;
    }

    }
return 0;}


uint8_t dht11_read_byte(void) {
    uint8_t i, byte = 0;
    for (i = 0; i < 8; i++) {
        while (!(GPIOA->IDR & (1 << 1))); // wait for high
        delays_us_tim2(40);
        if (GPIOA->IDR & (1 << 1)) {
            byte |= (1 << (7 - i)); // Read 1
            while (GPIOA->IDR & (1 << 1)); // wait for low
        }
    }
    return byte;
}
void dht11_read_data(uint8_t* temp, uint8_t* hum) {
    uint8_t h_int, h_dec, t_int, t_dec, checksum;

    dht11_start();
    if ( dht11_response()) {
        h_int = dht11_read_byte();
        h_dec = dht11_read_byte();
        t_int = dht11_read_byte();
        t_dec = dht11_read_byte();
        checksum = dht11_read_byte();
        
        if ((h_int + h_dec + t_int + t_dec) == checksum) {
            *hum = h_int;
            *temp = t_int;
        }
    }
}
// ------------ HC-SR04 Function ------------
uint16_t read_distance_cm(void) {
    uint32_t time = 0, timeout = 60000;

    GPIOA->ODR |= (1 << 2); // Trigger high
    delays_us_tim2(10);
    GPIOA->ODR &= ~(1 << 2); // Trigger low

    while (!(GPIOA->IDR & (1 << 3)) && timeout--) ; // Wait for Echo high
    if (timeout == 0) return 999; // Timeout (no object)

    TIM2->CNT = 0;
    timeout = 60000;
    while ((GPIOA->IDR & (1 << 3)) && timeout--) ; // Wait until Echo low
    if (timeout == 0) return 999;

    time = TIM2->CNT;
    return time / 58; // Convert to cm
}




// ------------ Main Program ------------
int main(void) {
    uint8_t temp = 0, hum = 0;
    uint16_t distance = 0;

    gpio_init();
    tim2_init();

    while (1) {
        dht11_read_data(&temp, &hum);
        distance = read_distance_cm();

        if (temp > 35 || hum < 30 || distance < 10) {
            GPIOA->ODR |= (1 << 5);  // Turn on alert LED
        } else {
            GPIOA->ODR &= ~(1 << 5); // Turn off alert
        }

        delay_ms(1000); // Delay between readings
    }
}

	
	