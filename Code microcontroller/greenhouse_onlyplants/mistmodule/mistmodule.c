#include "mistmodule.h"

	volatile bool toggle = false;
	
void mistmodule_init(void) {
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC->PCR[6] |= PORT_PCR_MUX(1);
	PORTC->PCR[0] |= PORT_PCR_MUX(1);
	PTC->PDDR |= (1<<6);
	PTC->PDDR &= ~(1<<0);
	PTC->PDOR |= (1<<6);
	if((PTC->PDIR & (1<<0)) == 0){
		PTC->PDOR &= ~(1<<6);
		delay_us(10000);
		PTC->PDOR |= (1<<6); 
	}
}	

void miston(bool miston){
	if(miston == true){
		if((PTC->PDIR & (1<<0)) == 1){
			PTC->PDOR &= ~(1<<6);
			delay_us(10000);
			PTC->PDOR |= (1<<6); 
		}
	}
	else if(miston == false){
		if((PTC->PDIR & (1<<0)) == 0){
			PTC->PDOR &= ~(1<<6);
			delay_us(10000);
			PTC->PDOR |= (1<<6);
			miston = false;
		}
	}
	
}	
