/*! ***************************************************************************
 *
 * \brief     Low level driver for the infrared proximity sensor
 * \file      ir.h
 * \author    Hugo Arends
 * \date      July 2021
 *
 * \copyright 2021 HAN University of Applied Sciences. All Rights Reserved.
 *            \n\n
 *            Permission is hereby granted, free of charge, to any person
 *            obtaining a copy of this software and associated documentation
 *            files (the "Software"), to deal in the Software without
 *            restriction, including without limitation the rights to use,
 *            copy, modify, merge, publish, distribute, sublicense, and/or sell
 *            copies of the Software, and to permit persons to whom the
 *            Software is furnished to do so, subject to the following
 *            conditions:
 *            \n\n
 *            The above copyright notice and this permission notice shall be
 *            included in all copies or substantial portions of the Software.
 *            \n\n
 *            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *            OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *            NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *            HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *            WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *            FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *            OTHER DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/
#include "soilmoisture.h"

#define input_min 15000 // minium measured value. 0% moisture
#define input_max 46100 // maximum measured value. 100% moisture

/*!
 * \brief Initialises the Soil moisture sensor
 *
 * This functions initializes the pin for the soil moisture sensor
 * - PTD6 is configured as an analog input (ADC channel 7b)
 */
void soil_init(void)
{
    // Enable clock to ADC0
    SIM->SCGC6 |= SIM_SCGC6_ADC0(1);
    
    // Enable clock to PORTs
    SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
    
    // The ouptut of the transistor is connectd to PTD6. Configure the pin as
    // ADC input pin (channel 7b).
    PORTD->PCR[6] &= ~0x7FF;
    
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

/*!
 * \brief Creates a blocking delay
 *
 * Software delay of approximately 1.02 us, depending on compiler version,
 * CPU clock frequency and optimizations.
 * - C compiler: ARMClang v6
 * - Language C: gnu11
 * - CPU clock: 48 MHz
 * - Optimization level: -O3
 * - Link-Time Optimization: disabled
 *
 * \param[in]  d  delay in microseconds
 */                              
static void delay_us(uint32_t d)
{

#if (CLOCK_SETUP != 1)
#warning This delay function does not work as designed
#endif

    volatile uint32_t t;

    for(t=4*d; t>0; t--)
    {
        __asm("nop");
        __asm("nop");
    }
}

/*!
 * \brief Samples the soil moisture sensor
 *
 * Take one samples from the soil moisture sensor.
 *
 * \return  The complemented 16-bit ADC value
 */
uint16_t soil_sample(void)
{
    //selecting channel B
    ADC0->CFG2 |= ADC_CFG2_MUXSEL_MASK;
  
    // Start a conversion on channel 7
    ADC0->SC1[0] = 7;
    
    // Wait for conversion to complete
    while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK))
    {}
        
    // Read the result
      uint16_t res = (uint16_t)ADC0->R[0];
        
    // Complement the result, because the capacitance increases with the moisture level
    // and we want the result to rise with increasing moisture level.
    return (0xFFFF - res);
      
    // reverting to channel A
    ADC0->CFG2 &= ~ADC_CFG2_MUXSEL_MASK;
}

/*!
 * \brief Maps the given value to a boundry
 *
 * Maps the sampled value and returns a maped value
 *
 * \param[in]  - x        Value you want to map
               - in_min   Minimum input value
               - in_max   Maximum input value
               - out_min  Minimum output value
               - out_max  Maximum output value
 *
 * \return  Maped value
 */

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;}

/*!
 * \brief Calculates the sampled value to a percentage
 *
 * Calculates the sampled value to a percentage.
 *
 * \param[in]  value  Active value adress
 *
 * \return  Moisture percentage
 */
void soil_measure(volatile int * value)
{   
    //*value = map(soil_sample(), input_min, input_max, 0, 100);
	*value = (soil_sample()-35000)*100/25000;
}
