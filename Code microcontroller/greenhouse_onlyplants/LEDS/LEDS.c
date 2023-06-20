#include "LEDS.h"

void leds_init(void){
//enable timer to timer
	SIM->SCGC6 |= SIM_SCGC6_TPM0(1);
	//enable timer for port D
	SIM->SCGC5 |= SIM_SCGC5_PORTC(1) | SIM_SCGC5_PORTE(1);
	
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	//select pwm
	TPM0->CONTROLS[4].CnV = 0;
	TPM0->CONTROLS[5].CnV = 0;

	TPM0->CONTROLS[4].CnSC = TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1);
	TPM0->CONTROLS[5].CnSC = TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1);

	//select timer in mux
	PORTE->PCR[31] = PORT_PCR_MUX(3);
	PORTC->PCR[9] = PORT_PCR_MUX(3);
	TPM0->MOD = 0xFFFF;
	//presclaler = 1
	TPM0->SC = TPM_SC_CMOD(1);
}
void leds_set(uint16_t speed,char n){
	switch(n){
		case 'r':
			TPM0->CONTROLS[4].CnV = speed;
		break;
		case 'b':
			TPM0->CONTROLS[5].CnV = speed;
		break;
		default:
		break;
	}}

