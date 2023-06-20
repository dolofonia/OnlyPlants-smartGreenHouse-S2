#include <MKL25Z4.h>
#include "uart0.h"
#include <stdio.h>
#include <math.h>
#include "HEATBED.h"

void termistor_init(void);
double term_sample(void);
static void delay_us(uint32_t d);
static uint16_t res;


int main(void){
	double result = 0;
	char string[16] = {"0"};
	heatbed_init();
	uart0_init();
	while(1){
		heatbed_set(65535);
		
//		sprintf(string,"%d\r",result);
//		uart0_send_string(string);
		
		delay_us(500000);
		sprintf(string,"T:%d\n\r",result);
		uart0_send_string(string);
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
