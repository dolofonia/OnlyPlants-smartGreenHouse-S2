#include "pump.h"

void pump_init(void){
	
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	PORTA->PCR[13] |= PORT_PCR_MUX(1);
	PTA->PDDR |= (1<<13);
	GPIOA->PDOR &= ~(1<<13);
}


void pump_en(bool state){
	
	if(state == true){
		GPIOA->PDOR |= (1<<13);
	}
	else
	{
		GPIOA->PDOR &= ~(1<<13);
	}
	
}
