
#ifndef _AHT21B_H
#define _AHT21B_H

#include "i2c0.h"
#include "controll.h"

bool aht21b_init(void);
void aht21b_getval(volatile int * temp,volatile int *hum);


#endif