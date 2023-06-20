#ifndef HEATBED_H
#define HEATBED_H
#include <stdint.h>
#include <MKL25Z4.h>
#include <math.h>
#include <stdbool.h>


extern volatile uint16_t result;
void heatbed_init(void);
void heatbed_set(uint16_t temp);
double term_sample(void);
void th_sample(void);
	
#endif
