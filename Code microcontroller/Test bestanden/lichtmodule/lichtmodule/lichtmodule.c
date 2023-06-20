# include "lichtmodule.h"
void ldr_init(void)
{
   // Enable clock to ADC0
    SIM->SCGC6 |= SIM_SCGC6_ADC0(1);
//   
    // Enable clock to PORTs
    SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
	
	
		// Mask PTD5
		PORTD->PCR[5] |= PORT_PCR_MUX(1);
		PORTD->PCR[5] &= ~0x7FF;
    
    // Configure ADC
    // - ADLPC = 1 : Low-power configuration. The power is reduced at the 
    //               expense of maximum clock speed.
    // - ADIV[1:0] = 00 : The divide ratio is 1 and the clock rate is input 
    //                    clock.
    // - ADLSMP = 1 : Long sample time.
    // - MODE[1:0] = 11 : Single-ended 16-bit conversion
    // - ADICLK[1:0] = 01 : (Bus clock)/2
    ADC0->CFG1 = 0x9D;
}
uint16_t ldr_sample(void)
{
	uint32_t temp = 0;
	uint16_t result = 0;
    // Start a conversion on channel 8
		ADC0->CFG2 |= ADC_CFG2_MUXSEL_MASK;
    ADC0->SC1[0] = ADC_SC1_ADCH(6);
    
    // Wait for conversion to complete
    while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
    {}
        
    // Read the result
    uint16_t res = (uint16_t)ADC0->R[0];
        
    // Complement the result, because the voltage falls with increasing IR level
    // and we want the result to rise with increasing IR level.
		ADC0->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;
		temp = ((10*pow(16000,(double)(1/0.7)))/pow((double)((655350000)/res)-10000,(double)(1/0.7)));
	  result = ((double)temp/20000) * 100;
    return (result);
}

	//ptd5 ADC0_SE6B
