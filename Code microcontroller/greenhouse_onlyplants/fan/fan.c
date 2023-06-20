#include "fan.h"


void fan_init(void){
	//enable timer to timer
	SIM->SCGC6 |= SIM_SCGC6_TPM0(1);
	//enable timer for port D
	SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
	
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	//select pwm
	TPM0->CONTROLS[0].CnV = 0;
	TPM0->CONTROLS[1].CnV = 0;
	TPM0->CONTROLS[2].CnV = 0;
	TPM0->CONTROLS[3].CnV = 0;
	
	TPM0->CONTROLS[0].CnSC = TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1);
	TPM0->CONTROLS[1].CnSC = TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1);
	TPM0->CONTROLS[2].CnSC = TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1);
	TPM0->CONTROLS[3].CnSC = TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1);

	//select timer in mux
	PORTD->PCR[0] = PORT_PCR_MUX(4);
	PORTD->PCR[1] = PORT_PCR_MUX(4);
	PORTD->PCR[2] = PORT_PCR_MUX(4);
	PORTD->PCR[3] = PORT_PCR_MUX(4);
	
	TPM0->MOD = 0xFFFF;
	//presclaler = 1
	TPM0->SC = TPM_SC_CMOD(1);
}
void fan_set(uint16_t speed,int n){
	if((n > 0) && (n < 5)){
		switch(n){
			case 1:
				TPM0->CONTROLS[0].CnV = speed;
				break;
			case 2:
				TPM0->CONTROLS[2].CnV = speed;
				break;
			case 3:
				TPM0->CONTROLS[3].CnV = speed;
				break;
			case 4:
				TPM0->CONTROLS[1].CnV = speed;
				break;
			default:break;
	}}}

