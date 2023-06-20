/*! ***************************************************************************
 *
 * \brief     Uart driver
 * \file      uart0.c
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
#include "uart0.h"

static queue_t TxQ, RxQ;

void uart2_init(void)
{  
    // enable clock to UART and Port A
    SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

    // select UART pins
    PORTE->PCR[23] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(4);
    PORTE->PCR[22] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(4);
  
    UART2->C2 &=  ~(UARTLP_C2_TE_MASK | UARTLP_C2_RE_MASK);
        
    // Set baud rate to baud rate
    uint32_t divisor = 24000000/(9600*16);
    UART2->BDH = UART_BDH_SBR(divisor>>8);
    UART2->BDL = UART_BDL_SBR(divisor);
    
    // No parity, 8 bits, one stop bit, other settings;
    UART2->C1 = 0; 
    UART2->S2 = 0;
    UART2->C3 = 0;
    
    // Enable transmitter and receiver but not interrupts
    UART2->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK;
    
    NVIC_SetPriority(UART2_IRQn, 128); // 0, 64, 128 or 192
    NVIC_ClearPendingIRQ(UART2_IRQn); 
    NVIC_EnableIRQ(UART2_IRQn);

    UART2->C2 |= UART_C2_RIE_MASK;
    
    q_init(&TxQ);
    q_init(&RxQ);  
}

void UART2_IRQHandler(void)
{
    uint8_t c;
  
    NVIC_ClearPendingIRQ(UART2_IRQn);
  
    if (UART2->S1 & UART_S1_TDRE_MASK)
    {
        // can send another character
        if(q_dequeue(&TxQ, &c))
        {
            UART2->D = c;
        } 
        else
        {
            // queue is empty so disable transmitter
            UART2->C2 &= ~UART_C2_TIE_MASK;
        }
    }
    if (UART2->S1 & UART_S1_RDRF_MASK)
    {
        c = UART2->D;
        
        if(!q_enqueue(&RxQ, c))
        {
            // error - queue full.
            while (1)
            {}
        }
    }
    if (UART2->S1 & (UART_S1_OR_MASK | UART_S1_NF_MASK | 
                     UART_S1_FE_MASK | UART_S1_PF_MASK))
    {
        // handle the error
        // clear the flag
    }
}

void uart2_send_string(char * str)
{   
    // Enqueue string
    while (*str != '\0') 
    { 
        // Wait for space to open up
					while(!q_enqueue(&TxQ, *str))
        {}
            
        str++;
    }
    
    // Start transmitter if it isn't already running
    if (!(UART2->C2 & UART_C2_TIE_MASK)) 
    {
        UART2->C2 |= UART_C2_TIE_MASK;
    }
}

uint32_t uart2_num_rx_chars_available(void)
{
    return q_size(&RxQ);
}

char uart2_get_char(void) 
{
    uint8_t c=0;
    
    // Wait for data.
    // If waiting is not desired, call the function
    // uart1_num_rx_chars_available() first to make sure data is available.
		if(uart2_num_rx_chars_available()>0){
    while(!q_dequeue(&RxQ, &c))
    {}
    }
    return (char)c;
}

void uart2_put_char(char c) 
{
    // Wait for space to open up
    while(!q_enqueue(&TxQ, c))
    {}
            
    // Start transmitter if it isn't already running
    if (!(UART2->C2 & UART_C2_TIE_MASK)) 
    {
        UART2->C2 |= UART_C2_TIE_MASK;
    }        
}
void uart2_get_string(char * stringout){
	char ctin = '0';
	int count = 0;
	if(uart2_num_rx_chars_available() > 0)
    {
			do{
				count = 0;
				do{
					ctin = uart2_get_char();
					stringout[count] = ctin;
					count++;
				}while((ctin != '\n'));
			}while(uart2_num_rx_chars_available() > 0);
			stringout[count] = '\0';
		}
}
void uart2_clear_buffer(void){
	q_emptyqueue(&RxQ);
}	
