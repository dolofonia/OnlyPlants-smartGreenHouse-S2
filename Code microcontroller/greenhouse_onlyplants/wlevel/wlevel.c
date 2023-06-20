#include "wlevel.h"


void wlevel_init(void)
{
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
		#define WSENS1     (12) // PTC12
		#define WSENS2     (13) // PTC13
		#define WSENS3     (16) // PTC16
		#define WSENS4     (17) // PTC17
		#define WSENS5     (16) // PTA16
		#define WSENS6     (17) // PTA17
		#define WSENS7     (5)  // PTA5
		#define WSENS8     (7)  // PTD7
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

		#define MASK(x)       (1UL << (x))	
		

		SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;	
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
		SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
		SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	
    PORTC->PCR[WSENS1] = 0b00100000000;
		PORTC->PCR[WSENS2] = 0b00100000000;
    PORTC->PCR[WSENS3] = 0b00100000000;
		PORTC->PCR[WSENS4] = 0b00100000000;
    PORTA->PCR[WSENS5] = 0b00100000000;
		PORTA->PCR[WSENS6] = 0b00100000000;	
		PORTA->PCR[WSENS7] = 0b00100000000;		
    PORTD->PCR[WSENS8] = 0b00100000000;	
	
// Set pins to inputs
		PTC->PDDR &= ~MASK(WSENS1);		
    PTC->PDDR &= ~MASK(WSENS2);
		PTC->PDDR &= ~MASK(WSENS3);		
    PTC->PDDR &= ~MASK(WSENS4);		
		PTA->PDDR &= ~MASK(WSENS5);		
    PTA->PDDR &= ~MASK(WSENS6);
		PTA->PDDR &= ~MASK(WSENS7);		
    PTD->PDDR &= ~MASK(WSENS8);
}
// Local function prototypes
int wlevel_sample(void)
{
	volatile int wlevel = 0;
	int wlevelper = 0;
        if(!(PTC->PDIR & MASK(WSENS1)))
        {
						wlevel = 8;
        }
        else if(!(PTC->PDIR & MASK(WSENS2)))
        {
            wlevel = 7;
        }       
				else if(!(PTC->PDIR & MASK(WSENS3)))
        {
            wlevel = 6;
        }
        else if(!(PTC->PDIR & MASK(WSENS4)))
        {
            wlevel = 5;
        }   
				else if(!(PTA->PDIR & MASK(WSENS5)))
        {
            wlevel = 4;
        }
        else if(!(PTA->PDIR & MASK(WSENS6)))
        {
            wlevel = 3;
        }   
				else if(!(PTA->PDIR & MASK(WSENS7)))
        {
            wlevel = 2;
        }
        else if(!(PTD->PDIR & MASK(WSENS8)))
        {
            wlevel = 1;
        }
				else
				{
						wlevel = 0;
				}
				wlevelper = (wlevel* 100)/8;
				
	return(wlevelper);
}

