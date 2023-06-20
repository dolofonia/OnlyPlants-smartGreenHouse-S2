 
#ifndef _MISTMODULE_H
#define _MISTMODULE_H

#include <MKL25Z4.h>
#include <stdbool.h>
//#include "controll.h"

void mistmodule_init(void);
void miston(bool miston);

void delay_us(uint32_t d);

extern volatile int test;
#endif 