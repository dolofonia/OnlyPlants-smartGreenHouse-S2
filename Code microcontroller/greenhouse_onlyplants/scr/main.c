#include "controll.h"


int main(void)
{
		system_init();
		fan_set(65535,1);
		while(1){
			menu_Update(); //update menu
			if(sample_flag){
				node_getint(targetval);//get last node red targetvallue 
				sample(); // get sensor values
				regulate(); // handle sensor values and set actuators
				node_sendstring(1); //send active vallues to node red
				sample_flag = false; // reset flag
			  
			}
			if(heatflag){
				heatflag = false;
				protect();
			}
		}
}

void HardFault_Handler(void){
	
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB->PCR[18] |= PORT_PCR_MUX(1);
	PORTB->PCR[19] |= PORT_PCR_MUX(1);
	PTB->PDDR |= (1<<18) | (1<<19);
	PTB->PDOR &= ~((1<<18) | (1<<19));
	NVIC_SystemReset();
	while(1){
		;
	}
}