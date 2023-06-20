/*! ***************************************************************************
 *
 * \brief     gpio project
 * \file      main.c
 * \author    Hugo Arends
 * \date      January 2023
 *
 * \copyright 2023 HAN University of Applied Sciences. All Rights Reserved.
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
#include <MKL25Z4.h>

#define RED_LED_POS   (18)  // PTB18
#define GREEN_LED_POS (19)  // PTB19

// For creating a mask
#define MASK(x)       (1UL << (x))

// Local function prototypes
static void delay_us(uint32_t d);

// Main
int main(void)
{
    // Enable clock to port B
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    // Configure all pins as follows:
    // - MUX[2:0] = 001 : Alternative 1 (GPIO)
    // - DSE = 0 : Low drive strength
    // - PFE = 0 : Passive input filter is disabled
    // - SRE = 0 : Fast slew rate is configured
    // - PE = 0 : Internal pullup or pulldown resistor is not enabled
    PORTB->PCR[18] = 0b00100000000;
    PORTB->PCR[19] = 0b00100000000;

    // Other instructions with the same result
  //PORTB->PCR[GREEN_LED_POS] = 0b00100000000; // Use define instead of number
  //PORTB->PCR[GREEN_LED_POS] = 0x100; // Hexadecimal notation
  //PORTB->PCR[GREEN_LED_POS] = 256; // Decimal notation
  //PORTB->PCR[GREEN_LED_POS] = (1<<8); // Using mask
  //PORTB->PCR[GREEN_LED_POS] = PORT_PCR_MUX(1); // Use define from MKL25Z4.h

    // Set pins to output
    // Notice the use of the |= operator here!
    // The purpose is to set the bits that are set in the mask in register PDDR
    // and leave all other bits unchanged
    PTB->PDDR |= MASK(RED_LED_POS);
    PTB->PDDR |= MASK(GREEN_LED_POS);

    // The GPIO peripheral can be accessed through either of two
    // pointers: PTB and GPIOB
  //GPIOB->PDDR |= MASK(RED_LED_POS);
  //GPIOB->PDDR |= MASK(GREEN_LED_POS);

    // Pin logic 1
    PTB->PSOR = MASK(RED_LED_POS);
    PTB->PSOR = MASK(GREEN_LED_POS);

    while(1)
    {
        // Toggle the pin and wait
        PTB->PTOR = MASK(GREEN_LED_POS);
        delay_us(100);

        // Toggle the pin and wait
        PTB->PTOR = MASK(GREEN_LED_POS);
        delay_us(999900);
    }
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
