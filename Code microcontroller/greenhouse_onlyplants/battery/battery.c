#include "battery.h"

void bat_init(void){
	SIM->SCGC6 |= SIM_SCGC6_ADC0(1);
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);
	ADC0->CFG1 = 0x9D;
	PORTB->PCR[8] |= PORT_PCR_MUX(1);
	
}
double bat_sample(void){
	double res;
	uint16_t samp;
	
	ADC0->SC1[0] = 23;
   
  // Wait for conversion to complete
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
    {}
       
  // Read the result
   samp = (uint16_t)ADC0->R[0];
	 res = (197.67*samp)/655350.0;
	 return res;
}
bool power_available(void){
	bool flag = false;
	if(PTB->PDIR & (1<<8))
		{
			flag = false;
		}
	else{
			flag = true;
		}
		return flag;
}
