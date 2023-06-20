#include "HEATBED.h"

static volatile bool overheat = false;
static volatile uint16_t settemp = 0;
static volatile bool heateron = false;
volatile uint16_t result = 0;
void heatbed_init(void){
	//enable timers
	SIM->SCGC6 |= SIM_SCGC6_TPM2(1);
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1);
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1);
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	SIM->SCGC6 |= SIM_SCGC6_ADC0(1);	
	SIM->SCGC6 |= SIM_SCGC6_PIT(1);
	//setup port configurations
	PORTB->PCR[2] = PORT_PCR_MUX(3); // select timer2 channel 0
	PORTB->PCR[0] &= ~0x7FF; // select ADC0 channel 0
	
	TPM2->CONTROLS[0].CnV = 0; //make sure the channel is set to LOW.
	TPM2->CONTROLS[0].CnSC = TPM_CnSC_MSB(1) | TPM_CnSC_ELSB(1); //select PWM and enable interupts
  TPM2->MOD = 0xFFFF; //set the top vallue
	TPM2->SC = TPM_SC_TOIE(1) | TPM_SC_CMOD(1); //enabe overflow interupts to activate the ADC
		
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;	
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(24e5-1);
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_CHN_MASK;
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	
	ADC0->CFG1 = 0x9D;  //bus clock/2 , single ended 16-bit conversion, long sample time, low power config
	//ADC0->SC2 |= ADC_SC2_ACFE_MASK | ADC_SC2_ACREN_MASK; //enable 
	//ADC0->CV2 = 43000;
	//ADC0->CV1 = 42000;
	//ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
	
	NVIC_SetPriority(PIT_IRQn, 192);	
	NVIC_ClearPendingIRQ(PIT_IRQn); 
	NVIC_EnableIRQ(PIT_IRQn);

	ADC0->SC1[0] |= 8;
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}
void heatbed_set(uint16_t temp){
	settemp = temp;
	if(temp == 0){
	heateron = false;
	}else{
	heateron = true;
	}
	if(!overheat){
		TPM2->CONTROLS[0].CnV = settemp;
	}
	
}
void protect(void){
	th_sample();
		v12_available = power_available();
		if (result > 51000) {
			overheat = true;   // Set the flag high if the result is greater than the CV
			TPM2->CONTROLS[0].CnV = 0;
		}
		else if(result < 50700){
			overheat = false;   // Set the flag low if the result is less than or equal to the CV
			if(heateron){
				TPM2->CONTROLS[0].CnV = settemp;
			}
		}
}

void PIT_IRQHandler(void)
{
   NVIC_ClearPendingIRQ(PIT_IRQn);
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK){
	
		PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
		heatflag = true;
	}
	if(PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK){
	
			PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;
			sample_flag = true;
			sendcounter++;
			errorcounter++;
			pumpcounter++;
			if(pumpcounter > 350){
				pumpcounter = 0;
			}
		
			if(errorcounter > 5){
				errorcounter = 0;
			}
			scrollBack_timer++;
	}
	
}
void th_sample(void){
	ADC0->SC1[0] = 8;
   
  // Wait for conversion to complete
	while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
    {}
       
  // Read the result
  result = (uint16_t)ADC0->R[0];
}
