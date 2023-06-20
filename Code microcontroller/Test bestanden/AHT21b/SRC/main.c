#include <MKL25Z4.h>
#include <stdio.h>

#include "aht21b.h"	
#include "uart0.h"
static void delay_us(uint32_t d);

int main(void){
	uart0_init();
	char test[17];
	int temp;
	int hum;
	aht21b_init();
	delay_us(100000);
	while(1){
		aht21b_getval(&temp,&hum);
		delay_us(100000);
		sprintf(test,"temp:%d\n\r",temp);
		uart0_send_string(test);
		sprintf(test,"hum:%d\n\r",hum);
		uart0_send_string(test);
	}
}
static void delay_us(uint32_t d)
{

#if (CLOCK_SETUP != 1)
#warning This delay function does not work as designed
#endif

    volatile uint32_t t;

    for(t=4*d; t>0; t--)
    {
        __asm("nop");
        __asm("nop");
    }
}
