#include <MKL25Z4.h>
#include "uart0.h"
#include <stdio.h>
#include "wlevel.h"
static void delay_us(uint32_t d);

int main(void){
	int wlevelper;
	char test[15];
	uart0_init();
	wlevel_intit();
	while(1){
		wlevelper = wlevel_meten();
		sprintf(test,"%d\n\r",wlevelper);
		uart0_send_string(test);
		delay_us(50000);
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
