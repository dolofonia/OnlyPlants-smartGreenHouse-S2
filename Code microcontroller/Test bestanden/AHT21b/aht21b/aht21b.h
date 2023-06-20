
#ifndef _AHT21B_H
#define _AHT21B_H

#include "i2c0.h"

bool aht21b_init(void);
void aht21b_getval(int * temp, int *hum);
static void delay_us(uint32_t d);




#endif