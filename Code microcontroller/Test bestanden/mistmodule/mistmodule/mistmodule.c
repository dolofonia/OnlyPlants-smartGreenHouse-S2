#include <mistmodule.h>
volatile int test = 0;


void mistmodule_init(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC->PCR[6] |= PORT_PCR_MUX(1);
	PORTC->PCR[0] |= PORT_PCR_MUX(1);
	PTC->PDDR |= (1UL<<6);
	delay_us(10000);
	if((PTC->PDIR & (1UL<<0)) == 0){
		PTC->PDOR &= ~(1UL<<6);
		delay_us(10000);
		PTC->PDOR |= (1UL<<6); 
	}
}	

void miston(bool miston){
		if((PTC->PDIR & (1UL<<0)) == 1){
			test = 1;
		}
		if((PTC->PDIR & (1UL<<0)) == 0){
			test = 0;
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
