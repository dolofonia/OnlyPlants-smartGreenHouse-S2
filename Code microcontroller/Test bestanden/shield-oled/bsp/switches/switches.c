#include "switches.h"

volatile bool sw1_pressed_flag = false; 
volatile bool sw2_pressed_flag = false; 
volatile bool sw3_pressed_flag = false; 
volatile bool sw4_pressed_flag = false; 
volatile bool sw5_pressed_flag = false; 


void sw_init(void){
		SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
		SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
    
    PORTA->PCR[4] = PORT_PCR_IRQC(10) | PORT_PCR_MUX(1);
    PORTA->PCR[12]= PORT_PCR_IRQC(10) | PORT_PCR_MUX(1);
    PORTA->PCR[2] = PORT_PCR_IRQC(10) | PORT_PCR_MUX(1);// | PORT_PCR_PS(1) | PORT_PCR_PE(1)
		PORTA->PCR[1] = PORT_PCR_IRQC(10) | PORT_PCR_MUX(1);
		PORTD->PCR[4] = PORT_PCR_IRQC(10) | PORT_PCR_MUX(1);	
    // Set port pin to input
    GPIOA->PDDR &= ~((1<<1) | (1<<2) | (1<<12) | (1<<4));
		GPIOD->PDDR &= ~(1<<4);
    
		// Enable interrupts with the lowest priority
		NVIC_SetPriority(PORTA_IRQn, 192);	
		NVIC_ClearPendingIRQ(PORTA_IRQn); 
		NVIC_EnableIRQ(PORTA_IRQn);
		NVIC_SetPriority(PORTD_IRQn, 192);	
		NVIC_ClearPendingIRQ(PORTD_IRQn); 
		NVIC_EnableIRQ(PORTD_IRQn);
	
}
void PORTA_IRQHandler(void)
{  
	// Clear pending interrupts
	

    // Is the Interrupt Status Flag set for PTD3?
    if((PORTA->ISFR & (1<<1)))
    {
        // Store the result
        sw1_pressed_flag = true;
        
        // Clear the flag
        PORTA->ISFR = (1<<1);
    }
		if((PORTA->ISFR & (1<<2)))
    {
        // Store the result
        sw2_pressed_flag = true;
        
        // Clear the flag
        PORTA->ISFR = (1<<2);
    }
		if((PORTA->ISFR & (1<<12)))
    {
        // Store the result
        sw4_pressed_flag = true;
        
        // Clear the flag
        PORTA->ISFR = (1<<12);
    }
		if((PORTA->ISFR & (1<<4)))
    {
        // Store the result
        sw5_pressed_flag = true;
        
        // Clear the flag
        PORTA->ISFR = (1<<4);
    }
		NVIC_ClearPendingIRQ(PORTA_IRQn);
}
void PORTD_IRQHandler(void){
		if((PORTD->ISFR & (1<<4)))
		{
				// Store the result
				sw3_pressed_flag = true;
        
				// Clear the flag
				PORTD->ISFR = (1<<4);
		}
		NVIC_ClearPendingIRQ(PORTD_IRQn);	
}
