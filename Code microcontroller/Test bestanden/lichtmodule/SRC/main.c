#include <MKL25Z4.h>
#include <stdbool.h>
#include <stdio.h>
#include "lichtmodule.h"
#include "uart0.h"
uint16_t range = 0;
void delay_us(uint32_t d);
int main(void){
	ldr_init();       
	uart0_init();
	char string[15];
    while(1)
    {
        // Repeat the loop every 100m
        
        range = ldr_sample();
				sprintf(string,"%d\n\r",range);
        uart0_send_string(string);
				delay_us(50000);
    }
	}

	void delay_us(uint32_t d)
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